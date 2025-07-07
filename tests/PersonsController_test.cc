#include <gtest/gtest.h>
#include <drogon/drogon.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Mapper.h>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include "../orgChartApi/controllers/PersonsController.h"
#include "../orgChartApi/models/Person.h"
#include "../orgChartApi/models/Job.h"
#include "../orgChartApi/models/Department.h"

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

class PersonsControllerTest : public ::testing::Test {
protected:
    std::shared_ptr<const PersonsController> controller;
    std::shared_ptr<DbClient> dbClient;
    std::mutex mtx;
    std::condition_variable cv;
    bool ready = false;

    void SetUp() override {
        controller = std::make_shared<PersonsController>();
        dbClient = drogon::app().getDbClient();

        // Clean up all test persons with possible last_names used in tests
        dbClient->execSqlSync("DELETE FROM person WHERE last_name LIKE 'Person%' OR last_name = 'Boss' OR last_name LIKE 'New%' OR last_name LIKE 'Updated%'");
        dbClient->execSqlSync("DELETE FROM department WHERE id IN (1,2)");
        dbClient->execSqlSync("DELETE FROM job WHERE id IN (10,20)");

        dbClient->execSqlSync("INSERT INTO department(id, name) VALUES (1, 'Engineering'), (2, 'Finance') ON CONFLICT (id) DO UPDATE SET name = EXCLUDED.name;");
        dbClient->execSqlSync("INSERT INTO job(id, title) VALUES (10, 'Engineer'), (20, 'Manager') ON CONFLICT (id) DO UPDATE SET title = EXCLUDED.title;");

        // Insert manager and direct reports with unique last_names
        dbClient->execSqlSync(
            "INSERT INTO person(id, job_id, department_id, manager_id, first_name, last_name, hire_date) VALUES "
            "(1002, 20, 1, NULL, 'Manager', 'Boss', '2020-01-01'),"
            "(1001, 10, 1, 1002, 'Test', 'Person', '2023-01-01'),"
            "(1003, 10, 2, 1002, 'Second', 'Person2', '2023-02-01') "
            "ON CONFLICT (id) DO UPDATE SET first_name = EXCLUDED.first_name, last_name = EXCLUDED.last_name;"
        );
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

Person makePerson(int id, int jobId, int deptId, int managerId, const std::string& first, const std::string& last) {
    Json::Value json;
    json["id"] = id;
    json["job_id"] = jobId;
    json["department_id"] = deptId;
    json["manager_id"] = managerId;
    json["first_name"] = first;
    json["last_name"] = last;
    json["hire_date"] = "2024-01-01";
    return Person(json);
}

TEST_F(PersonsControllerTest, GetPersonsSuccess) {
    HttpRequestPtr request = HttpRequest::newHttpRequest();
    request->setParameter("offset", "0");
    request->setParameter("limit", "10");
    request->setParameter("sort_field", "id");
    request->setParameter("sort_order", "asc");

    auto callback = [this](const HttpResponsePtr& response) {
        ASSERT_EQ(response->getStatusCode(), HttpStatusCode::k200OK);
        auto json = response->getJsonObject();
        ASSERT_TRUE(json);
        EXPECT_TRUE(json->isArray());
        EXPECT_GE(json->size(), 1);
        notifyCallbackDone();
    };

    controller->get(request, std::move(callback));
    waitForCallback();
}

TEST_F(PersonsControllerTest, GetPersonByIdSuccess) {
    int personId = 1001;
    HttpRequestPtr request = HttpRequest::newHttpRequest();

    auto callback = [this, personId](const HttpResponsePtr& response) {
        ASSERT_EQ(response->getStatusCode(), HttpStatusCode::k200OK);
        auto json = response->getJsonObject();
        ASSERT_TRUE(json);
        EXPECT_EQ((*json)["id"].asInt(), personId);
        notifyCallbackDone();
    };

    controller->getOne(request, std::move(callback), personId);
    waitForCallback();
}

TEST_F(PersonsControllerTest, GetPersonByIdNotFound) {
    int personId = 2001;
    HttpRequestPtr request = HttpRequest::newHttpRequest();

    auto callback = [this](const HttpResponsePtr& response) {
        ASSERT_EQ(response->getStatusCode(), HttpStatusCode::k404NotFound);
        notifyCallbackDone();
    };

    controller->getOne(request, std::move(callback), personId);
    waitForCallback();
}

TEST_F(PersonsControllerTest, CreatePersonSuccess) {
    // Use a unique last_name
    std::string uniqueLastName = "NewPerson_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    HttpRequestPtr request = HttpRequest::newHttpRequest();
    Person newPerson = makePerson(2002, 10, 1, 1002, "New", uniqueLastName);

    auto callback = [this, uniqueLastName](const HttpResponsePtr& response) {
        ASSERT_EQ(response->getStatusCode(), HttpStatusCode::k201Created);
        auto json = response->getJsonObject();
        ASSERT_TRUE(json);
        EXPECT_EQ((*json)["last_name"].asString(), uniqueLastName);
        notifyCallbackDone();
    };

    controller->createOne(request, std::move(callback), std::move(newPerson));
    waitForCallback();
}

TEST_F(PersonsControllerTest, UpdatePersonSuccess) {
    int personId = 1001;
    // Use a unique last_name for update
    std::string uniqueLastName = "UpdatedPerson_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    HttpRequestPtr request = HttpRequest::newHttpRequest();
    Person updateDetails = makePerson(personId, 10, 2, 1002, "Updated", uniqueLastName);

    auto callback = [this](const HttpResponsePtr& response) {
        ASSERT_EQ(response->getStatusCode(), HttpStatusCode::k204NoContent);
        notifyCallbackDone();
    };

    controller->updateOne(request, std::move(callback), personId, std::move(updateDetails));
    waitForCallback();
}

TEST_F(PersonsControllerTest, DeletePersonSuccess) {
    int personId = 1003;
    HttpRequestPtr request = HttpRequest::newHttpRequest();

    auto callback = [this](const HttpResponsePtr& response) {
        ASSERT_EQ(response->getStatusCode(), HttpStatusCode::k204NoContent);
        notifyCallbackDone();
    };

    controller->deleteOne(request, std::move(callback), personId);
    waitForCallback();
}

TEST_F(PersonsControllerTest, GetDirectReportsSuccess) {
    int managerId = 1002;
    HttpRequestPtr request = HttpRequest::newHttpRequest();

    auto callback = [this](const HttpResponsePtr& response) {
        ASSERT_EQ(response->getStatusCode(), HttpStatusCode::k200OK);
        auto json = response->getJsonObject();
        ASSERT_TRUE(json);
        EXPECT_TRUE(json->isArray());
        EXPECT_GE(json->size(), 1);
        notifyCallbackDone();
    };

    controller->getDirectReports(request, std::move(callback), managerId);
    waitForCallback();
}
