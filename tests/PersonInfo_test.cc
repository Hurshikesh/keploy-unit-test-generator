#include <gtest/gtest.h>
#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include "../orgChartApi/models/PersonInfo.h"

using namespace drogon;
using namespace drogon_model::org_chart;

// Adjust these values to match your test database contents!
const int TEST_ID = 1;
const int TEST_JOB_ID = 10;
const std::string TEST_JOB_TITLE = "Engineer";
const int TEST_DEPARTMENT_ID = 20;
const std::string TEST_DEPARTMENT_NAME = "Engineering";
const int TEST_MANAGER_ID = 30;
const std::string TEST_MANAGER_FULL_NAME = "Alice Smith";
const std::string TEST_FIRST_NAME = "John";
const std::string TEST_LAST_NAME = "Doe";
const std::string TEST_HIRE_DATE = "2023-01-01";

TEST(PersonInfoIntegration, ConstructFromDbRow) {
    // Make sure Drogon app is initialized and running before this test!
    auto dbClient = drogon::app().getDbClient();
    // Make sure your test DB has a row with id=TEST_ID and the expected values!
    auto result = dbClient->execSqlSync("SELECT * FROM person_info WHERE id=$1", TEST_ID);
    ASSERT_FALSE(result.empty());
    const drogon::orm::Row &row = result[0];
    PersonInfo info(row, -1);

    EXPECT_EQ(info.getValueOfId(), TEST_ID);
    EXPECT_EQ(info.getValueOfJobId(), TEST_JOB_ID);
    EXPECT_EQ(info.getValueOfJobTitle(), TEST_JOB_TITLE);
    EXPECT_EQ(info.getValueOfDepartmentId(), TEST_DEPARTMENT_ID);
    EXPECT_EQ(info.getValueOfDepartmentName(), TEST_DEPARTMENT_NAME);
    EXPECT_EQ(info.getValueOfManagerId(), TEST_MANAGER_ID);
    EXPECT_EQ(info.getValueOfManagerFullName(), TEST_MANAGER_FULL_NAME);
    EXPECT_EQ(info.getValueOfFirstName(), TEST_FIRST_NAME);
    EXPECT_EQ(info.getValueOfLastName(), TEST_LAST_NAME);
    EXPECT_EQ(info.getValueOfHireDate().toDbStringLocal(), TEST_HIRE_DATE);

    // Test toJson output as well
    Json::Value json = info.toJson();
    EXPECT_EQ(json["id"].asInt(), TEST_ID);
    EXPECT_EQ(json["job_id"].asInt(), TEST_JOB_ID);
    EXPECT_EQ(json["job_title"].asString(), TEST_JOB_TITLE);
    EXPECT_EQ(json["department_id"].asInt(), TEST_DEPARTMENT_ID);
    EXPECT_EQ(json["department_name"].asString(), TEST_DEPARTMENT_NAME);
    EXPECT_EQ(json["manager_id"].asInt(), TEST_MANAGER_ID);
    EXPECT_EQ(json["manager_full_name"].asString(), TEST_MANAGER_FULL_NAME);
    EXPECT_EQ(json["first_name"].asString(), TEST_FIRST_NAME);
    EXPECT_EQ(json["last_name"].asString(), TEST_LAST_NAME);
    EXPECT_EQ(json["hire_date"].asString(), TEST_HIRE_DATE);
}
TEST(PersonInfoIntegration, NoRowFound) {
    auto dbClient = drogon::app().getDbClient();
    auto result = dbClient->execSqlSync("SELECT * FROM person_info WHERE id=$1", -9999); // unlikely id
    ASSERT_TRUE(result.empty());
}
