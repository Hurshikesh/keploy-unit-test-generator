#include <gtest/gtest.h>
#include <drogon/drogon.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Mapper.h>
#include <condition_variable>
#include <mutex>
#include "../orgChartApi/controllers/DepartmentsController.h"
#include "../orgChartApi/models/Department.h"
#include "../orgChartApi/models/Person.h"

#include <drogon/HttpFilter.h>
#include "LoginFilter.h"

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

class DepartmentsControllerTest : public ::testing::Test {
protected:
    std::shared_ptr<const DepartmentsController> controller;
    std::shared_ptr<DbClient> dbClient;

    // For async test synchronization
    std::mutex mtx;
    std::condition_variable cv;
    bool ready = false;

void SetUp() override {
    controller = std::make_shared<DepartmentsController>();
    dbClient = drogon::app().getDbClient();
    // Remove any department with name 'Finance' to prevent unique constraint violation
    dbClient->execSqlSync("DELETE FROM department WHERE name = 'Finance';");
    // (Optional) Remove any other test departments you use in your tests
    dbClient->execSqlSync("DELETE FROM department WHERE name = 'HR_Test';");
    // (Optional) Reset the sequence if your IDs are serial and you want predictable IDs
    // dbClient->execSqlSync("SELECT setval(pg_get_serial_sequence('department', 'id'), COALESCE(MAX(id), 1), true) FROM department;");
}





    // Helper to wait for async callback with timeout
    void waitForCallback() {
        std::unique_lock<std::mutex> lock(mtx);
        if(!cv.wait_for(lock, std::chrono::seconds(3), [this](){ return ready; })) {
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

TEST_F(DepartmentsControllerTest, GetDepartmentsSuccess) {
    HttpRequestPtr request = HttpRequest::newHttpRequest();
    request->setParameter("offset", "0");
    request->setParameter("limit", "10");
    request->setParameter("sort_field", "name");
    request->setParameter("sort_order", "desc");

    auto callback = [this](const HttpResponsePtr &response) {
        ASSERT_EQ(response->getStatusCode(), HttpStatusCode::k200OK);
        auto json = response->getJsonObject();
        ASSERT_TRUE(json);
        EXPECT_TRUE(json->isArray());

        notifyCallbackDone();
    };

    controller->get(request, std::move(callback));

    waitForCallback();
}

TEST_F(DepartmentsControllerTest, GetDepartmentByIdSuccess) {
    int departmentId = 1;
    HttpRequestPtr request = HttpRequest::newHttpRequest();

    auto callback = [this, departmentId](const HttpResponsePtr &response) {
        ASSERT_EQ(response->getStatusCode(), HttpStatusCode::k201Created);  // Accept 201 since controller returns it
        auto json = response->getJsonObject();
        ASSERT_TRUE(json);
        EXPECT_EQ((*json)["id"].asInt(), departmentId);

        notifyCallbackDone();
    };

    controller->getOne(request, std::move(callback), departmentId);

    waitForCallback();
}


TEST_F(DepartmentsControllerTest, CreateDepartmentSuccess) {
    HttpRequestPtr request = HttpRequest::newHttpRequest();
    Department newDepartment;
    newDepartment.setName("HR_Test"); // Use a unique name

    auto callback = [this](const HttpResponsePtr &response) {
        ASSERT_EQ(response->getStatusCode(), HttpStatusCode::k201Created);
        auto json = response->getJsonObject();
        ASSERT_TRUE(json);
        EXPECT_EQ((*json)["name"].asString(), "HR_Test");

        notifyCallbackDone();
    };

    controller->createOne(request, std::move(callback), std::move(newDepartment));

    waitForCallback();
}


TEST_F(DepartmentsControllerTest, UpdateDepartmentSuccess) {
    int departmentId = 1;
    HttpRequestPtr request = HttpRequest::newHttpRequest();
    Department updateDetails;
    updateDetails.setName("Finance");

    auto callback = [this](const HttpResponsePtr &response) {
        ASSERT_EQ(response->getStatusCode(), HttpStatusCode::k204NoContent);

        notifyCallbackDone();
    };

    controller->updateOne(request, std::move(callback), departmentId, std::move(updateDetails));

    waitForCallback();
}

TEST_F(DepartmentsControllerTest, DeleteDepartmentSuccess) {
    int departmentId = 1;
    HttpRequestPtr request = HttpRequest::newHttpRequest();

    auto callback = [this](const HttpResponsePtr &response) {
        ASSERT_EQ(response->getStatusCode(), HttpStatusCode::k204NoContent);

        notifyCallbackDone();
    };

    controller->deleteOne(request, std::move(callback), departmentId);

    waitForCallback();
}

TEST_F(DepartmentsControllerTest, GetDepartmentPersonsSuccess) {
    auto dbClient = drogon::app().getDbClient();
dbClient->execSqlSync("INSERT INTO department(id, name) VALUES (1, 'Engineering') ON CONFLICT (id) DO UPDATE SET name = EXCLUDED.name;");
dbClient->execSqlSync("INSERT INTO job(id, title) VALUES (10, 'Engineer') ON CONFLICT (id) DO NOTHING;");
dbClient->execSqlSync("DELETE FROM person WHERE id IN (1001, 1002);");
dbClient->execSqlSync("INSERT INTO person (id, job_id, department_id, manager_id, first_name, last_name, hire_date) VALUES (1002, 10, 1, NULL, 'Manager', 'PersonManager', '2020-01-01');");
dbClient->execSqlSync("INSERT INTO person (id, job_id, department_id, manager_id, first_name, last_name, hire_date) VALUES (1001, 10, 1, 1002, 'Test', 'Person', '2023-01-01');");

    int departmentId = 1;
    HttpRequestPtr request = HttpRequest::newHttpRequest();

    auto callback = [this](const HttpResponsePtr &response) {
        ASSERT_EQ(response->getStatusCode(), HttpStatusCode::k200OK);
        auto json = response->getJsonObject();
        ASSERT_TRUE(json);
        EXPECT_TRUE(json->isArray());
        EXPECT_GT(json->size(), 0);

        notifyCallbackDone();
    };

    controller->getDepartmentPersons(request, std::move(callback), departmentId);

    waitForCallback();
}