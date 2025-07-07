#include <gtest/gtest.h>
#include "../orgChartApi/models/Department.h"
#include <json/json.h>
#include <drogon/orm/Result.h>
#include <drogon/orm/Row.h>
#include <memory>
#include <string>

using namespace drogon_model::org_chart;

// Test Suite for Department Class
class DepartmentTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Any setup code if needed
    }

    void TearDown() override
    {
        // Any teardown code if needed
    }
};

TEST(DepartmentTest, ConstructorFromJson)
{
    Json::Value validJson;
    validJson["id"] = 5;
    validJson["name"] = "Finance";

    Department department(validJson);
    EXPECT_EQ(department.getValueOfId(), 5);
    EXPECT_EQ(department.getValueOfName(), "Finance");

    Json::Value partialJson;
    partialJson["name"] = "Marketing";

    Department departmentPartial(partialJson);
    EXPECT_EQ(*departmentPartial.getName(), "Marketing");
    EXPECT_EQ(departmentPartial.getId(), nullptr);
}

// -------- REMOVED ConstructorFromRow TEST --------
// You cannot construct drogon::orm::Row directly for unit testing.
// TEST(DepartmentTest, ConstructorFromRow)
// {
//     drogon::orm::Row row;
//     row["id"] = 2;
//     row["name"] = std::string("Engineering");

//     Department department(row, -1);
//     EXPECT_EQ(department.getValueOfId(), 2);
//     EXPECT_EQ(department.getValueOfName(), "Engineering");
// }

TEST(DepartmentTest, SetGetName)
{
    Department department;
    EXPECT_TRUE(department.getName() == nullptr);
    EXPECT_EQ(department.getValueOfName(), "");

    department.setName("Human Resources");
    EXPECT_EQ(department.getValueOfName(), "Human Resources");
    EXPECT_TRUE(department.getName() != nullptr);
}

TEST(DepartmentTest, SetGetId)
{
    Department department;
    EXPECT_TRUE(department.getId() == nullptr);
    EXPECT_EQ(department.getValueOfId(), 0);

    department.setId(10);
    EXPECT_EQ(department.getValueOfId(), 10);
    EXPECT_TRUE(department.getId() != nullptr);
}

TEST(DepartmentTest, ToJson)
{
    Department department;
    department.setId(15);
    department.setName("Design");

    auto json = department.toJson();
    ASSERT_TRUE(json.isObject());
    EXPECT_EQ(json["id"].asInt(), 15);
    EXPECT_EQ(json["name"].asString(), "Design");
}

TEST(DepartmentTest, ValidateJsonForCreation)
{
    Department department;
    Json::Value validJson;
    validJson["name"] = "Sales";

    std::string err;
    bool result = department.validateJsonForCreation(validJson, err);
    EXPECT_TRUE(result);
    EXPECT_TRUE(err.empty());

    Json::Value invalidJson;  // Missing name field
    result = department.validateJsonForCreation(invalidJson, err);
    EXPECT_FALSE(result);
    EXPECT_EQ(err, "The name column cannot be null");
}

TEST(DepartmentTest, ValidateJsonForUpdate)
{
    Department department;
    Json::Value validJson;
    validJson["id"] = 8;
    validJson["name"] = "Research";

    std::string err;
    bool result = department.validateJsonForUpdate(validJson, err);
    EXPECT_TRUE(result);
    EXPECT_TRUE(err.empty());

    Json::Value invalidJson;  // Missing id field
    result = department.validateJsonForUpdate(invalidJson, err);
    EXPECT_FALSE(result);
    EXPECT_EQ(err, "The value of primary key must be set in the json object for update");
}

TEST(DepartmentTest, ValidateMasqueradedJsonForCreation)
{
    Department department;
    std::vector<std::string> masqueradingVector{"custom_id", "custom_name"};
    Json::Value validJson;
    validJson["custom_name"] = "Operations";

    std::string err;
    bool result = department.validateMasqueradedJsonForCreation(validJson, masqueradingVector, err);
    EXPECT_TRUE(result);
    EXPECT_TRUE(err.empty());

    Json::Value invalidJson;  // Missing custom_name field
    result = department.validateMasqueradedJsonForCreation(invalidJson, masqueradingVector, err);
    EXPECT_FALSE(result);
    EXPECT_EQ(err, "The custom_name column cannot be null");
}

TEST(DepartmentTest, ValidateMasqueradedJsonForUpdate)
{
    Department department;
    std::vector<std::string> masqueradingVector{"custom_id", "custom_name"};
    Json::Value validJson;
    validJson["custom_id"] = 12;
    validJson["custom_name"] = "HR";

    std::string err;
    bool result = department.validateMasqueradedJsonForUpdate(validJson, masqueradingVector, err);
    EXPECT_TRUE(result);
    EXPECT_TRUE(err.empty());

    Json::Value invalidJson;
    result = department.validateMasqueradedJsonForUpdate(invalidJson, masqueradingVector, err);
    EXPECT_FALSE(result);
    EXPECT_EQ(err, "The value of primary key must be set in the json object for update");
}

TEST(DepartmentTest, ToMasqueradedJson)
{
    Department department;
    department.setId(20);
    department.setName("Accounts");

    std::vector<std::string> masqueradingVector{"custom_id", "custom_name"};
    auto json = department.toMasqueradedJson(masqueradingVector);

    ASSERT_TRUE(json.isObject());
    EXPECT_EQ(json["custom_id"].asInt(), 20);
    EXPECT_EQ(json["custom_name"].asString(), "Accounts");
}
