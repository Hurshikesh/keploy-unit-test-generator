#include <gtest/gtest.h>
#include <drogon/drogon.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Mapper.h>
#include <condition_variable>
#include <mutex>
#include "../orgChartApi/controllers/JobsController.h"
#include "../orgChartApi/models/Job.h"
#include "../orgChartApi/models/Person.h"

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

class JobsControllerTest : public ::testing::Test {
protected:
    std::shared_ptr<const JobsController> controller;
    std::shared_ptr<DbClient> dbClient;
    std::mutex mtx;
    std::condition_variable cv;
    bool ready = false;

    void SetUp() override {
        controller = std::make_shared<JobsController>();
        dbClient = drogon::app().getDbClient();
        dbClient->execSqlSync("DELETE FROM job WHERE id IN (1,2,3,1001)");
    }

    void waitForCallback() {
        std::unique_lock<std::mutex> lock(mtx);
        if (!cv.wait_for(lock, std::chrono::seconds(3), [this](){ return ready; })) {
            FAIL() << "Timeout waiting for callback";
        }
        ready = false;
    }

    void notifyCallbackDone() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            ready = true;
        }
        cv.notify_one();
    }
};

Job makeJob(int id, const std::string& title) {
    Json::Value json;
    json["id"] = id;
    json["title"] = title;
    return Job(json);
}

TEST_F(JobsControllerTest, GetJobByIdSuccess) {
    dbClient->execSqlSync("INSERT INTO job(id, title) VALUES (1, 'Mock Title') ON CONFLICT (id) DO UPDATE SET title = EXCLUDED.title;");

    int jobId = 1;
    HttpRequestPtr request = HttpRequest::newHttpRequest();

    auto callback = [this, jobId](const HttpResponsePtr& response) {
        ASSERT_EQ(response->getStatusCode(), HttpStatusCode::k201Created);
        auto json = response->getJsonObject();
        ASSERT_TRUE(json);
        EXPECT_EQ((*json)["id"].asInt(), jobId);
        EXPECT_EQ((*json)["title"].asString(), "Mock Title");
        notifyCallbackDone();
    };

    controller->getOne(request, std::move(callback), jobId);
    waitForCallback();
}

TEST_F(JobsControllerTest, CreateJobSuccess) {
    // Clean up any existing job with this title
    dbClient->execSqlSync("DELETE FROM job WHERE title = 'Test Job'");

    // (Optional) Use a unique title to avoid collisions in parallel runs
    std::string uniqueTitle = "Test Job " + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());

    Job newJob;
    newJob.setTitle(uniqueTitle);

    HttpRequestPtr request = HttpRequest::newHttpRequest();

    auto callback = [this, uniqueTitle](const HttpResponsePtr& response) {
        ASSERT_EQ(response->getStatusCode(), HttpStatusCode::k201Created);
        auto json = response->getJsonObject();
        ASSERT_TRUE(json);
        EXPECT_EQ((*json)["title"].asString(), uniqueTitle);
        notifyCallbackDone();
    };

    controller->createOne(request, std::move(callback), std::move(newJob));
    waitForCallback();
}


TEST_F(JobsControllerTest, UpdateJobSuccess) {
    // Clean up and insert the job to update
    dbClient->execSqlSync("DELETE FROM job WHERE id = 2 OR title = 'Updated Title'");
    dbClient->execSqlSync("INSERT INTO job(id, title) VALUES (2, 'Old Title') ON CONFLICT (id) DO UPDATE SET title = EXCLUDED.title;");

    int jobId = 2;

    // Prepare update JSON
    Json::Value updateJson;
    updateJson["title"] = "Updated Title";
    // If your controller needs "id" in the JSON, add: updateJson["id"] = jobId;

    HttpRequestPtr request = HttpRequest::newHttpJsonRequest(updateJson);

    auto callback = [this](const HttpResponsePtr& response) {
        ASSERT_EQ(response->getStatusCode(), HttpStatusCode::k204NoContent);
        notifyCallbackDone();
    };

    // Call with the correct arguments
    controller->updateOne(request, std::move(callback), jobId, Job(updateJson));
    waitForCallback();
}


TEST_F(JobsControllerTest, DeleteJobSuccess) {
    dbClient->execSqlSync("INSERT INTO job(id, title) VALUES (3, 'Delete Me') ON CONFLICT (id) DO UPDATE SET title = EXCLUDED.title;");
    int jobId = 3;
    HttpRequestPtr request = HttpRequest::newHttpRequest();

    auto callback = [this](const HttpResponsePtr& response) {
        ASSERT_EQ(response->getStatusCode(), HttpStatusCode::k204NoContent);
        notifyCallbackDone();
    };

    controller->deleteOne(request, std::move(callback), jobId);
    waitForCallback();
}

TEST_F(JobsControllerTest, GetJobByIdNotFound) {
    int jobId = 1001;
    HttpRequestPtr request = HttpRequest::newHttpRequest();

    auto callback = [this](const HttpResponsePtr& response) {
        ASSERT_EQ(response->getStatusCode(), HttpStatusCode::k404NotFound);
        notifyCallbackDone();
    };

    controller->getOne(request, std::move(callback), jobId);
    waitForCallback();
}
