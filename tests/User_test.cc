#include <gtest/gtest.h>
#include <json/json.h>
#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include "../orgChartApi/models/User.h"

using namespace drogon_model::org_chart;

// ----------- UNIT TESTS -----------

TEST(UserTest, GetColumnNameValidIndex) {
    EXPECT_EQ(User::getColumnName(0), "id");
    EXPECT_EQ(User::getColumnName(1), "username");
    EXPECT_EQ(User::getColumnName(2), "password");
}

TEST(UserTest, GetColumnNameInvalidIndex) {
    GTEST_SKIP() << "User::getColumnName uses assert, cannot test for exception.";
}

TEST(UserTest, ConstructorFromJson) {
    Json::Value mockJson;
    mockJson["id"] = 20;
    mockJson["username"] = "json_user";
    mockJson["password"] = "json_password";

    User user(mockJson);
    EXPECT_EQ(user.getValueOfId(), 20);
    EXPECT_EQ(user.getValueOfUsername(), "json_user");
    EXPECT_EQ(user.getValueOfPassword(), "json_password");
}

TEST(UserTest, UpdateByJson) {
    Json::Value mockJson;
    mockJson["id"] = 30;
    mockJson["username"] = "updated_user";
    mockJson["password"] = "updated_password";

    User user;
    user.updateByJson(mockJson);

    EXPECT_EQ(user.getValueOfId(), 30);
    EXPECT_EQ(user.getValueOfUsername(), "updated_user");
    EXPECT_EQ(user.getValueOfPassword(), "updated_password");
}

TEST(UserTest, Setters) {
    User user;
    user.setId(40);
    EXPECT_EQ(user.getValueOfId(), 40);

    user.setUsername("setter_user");
    EXPECT_EQ(user.getValueOfUsername(), "setter_user");

    user.setPassword("setter_password");
    EXPECT_EQ(user.getValueOfPassword(), "setter_password");
}

TEST(UserTest, ValidationJsonForCreation) {
    Json::Value mockJson;
    std::string error;

    mockJson["username"] = "valid_user";
    mockJson["password"] = "valid_password";

    EXPECT_TRUE(User::validateJsonForCreation(mockJson, error));
    EXPECT_EQ(error, "");

    mockJson.removeMember("username");
    EXPECT_FALSE(User::validateJsonForCreation(mockJson, error));
    EXPECT_EQ(error, "The username column cannot be null");
}

TEST(UserTest, ValidationJsonForUpdate) {
    Json::Value mockJson;
    std::string error;

    mockJson["id"] = 50;
    mockJson["username"] = "valid_update_user";
    mockJson["password"] = "valid_update_password";

    EXPECT_TRUE(User::validateJsonForUpdate(mockJson, error));
    EXPECT_EQ(error, "");

    mockJson.removeMember("id");
    EXPECT_FALSE(User::validateJsonForUpdate(mockJson, error));
    EXPECT_EQ(error, "The value of primary key must be set in the json object for update");
}

TEST(UserTest, ToJson) {
    User user;
    user.setId(60);
    user.setUsername("json_output_user");
    user.setPassword("json_output_password");

    Json::Value jsonOutput = user.toJson();
    EXPECT_EQ(jsonOutput["id"].asInt(), 60);
    EXPECT_EQ(jsonOutput["username"].asString(), "json_output_user");
    EXPECT_EQ(jsonOutput["password"].asString(), "json_output_password");
}

// ----------- INTEGRATION TEST (REAL DB) -----------

const int TEST_USER_ID = 1001;
const std::string TEST_USERNAME = "integration_user";
const std::string TEST_PASSWORD = "integration_pw";

class UserIntegrationTest : public ::testing::Test {
protected:
    std::shared_ptr<drogon::orm::DbClient> dbClient;

    void SetUp() override {
        dbClient = drogon::app().getDbClient();
        // Clean up any existing test user
        dbClient->execSqlSync("DELETE FROM \"user\" WHERE id = $1", TEST_USER_ID);
        // Insert the test user
        dbClient->execSqlSync("INSERT INTO \"user\" (id, username, password) VALUES ($1, $2, $3)",
                              TEST_USER_ID, TEST_USERNAME, TEST_PASSWORD);
    }

    void TearDown() override {
        // Clean up after test
        dbClient->execSqlSync("DELETE FROM \"user\" WHERE id = $1", TEST_USER_ID);
    }
};

TEST_F(UserIntegrationTest, ConstructFromDbRow) {
    auto result = dbClient->execSqlSync("SELECT * FROM \"user\" WHERE id=$1", TEST_USER_ID);
    ASSERT_FALSE(result.empty());
    const drogon::orm::Row &row = result[0];
    User user(row, -1);

    EXPECT_EQ(user.getValueOfId(), TEST_USER_ID);
    EXPECT_EQ(user.getValueOfUsername(), TEST_USERNAME);
    EXPECT_EQ(user.getValueOfPassword(), TEST_PASSWORD);

    Json::Value json = user.toJson();
    EXPECT_EQ(json["id"].asInt(), TEST_USER_ID);
    EXPECT_EQ(json["username"].asString(), TEST_USERNAME);
    EXPECT_EQ(json["password"].asString(), TEST_PASSWORD);
}
