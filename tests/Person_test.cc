#include <gtest/gtest.h>
#include <json/json.h>
#include <trantor/utils/Date.h>
#include "../orgChartApi/models/Person.h"

using namespace drogon_model::org_chart;

TEST(PersonTest, ConstructorFromValidJson)
{
    Json::Value json;
    json["id"] = 1;
    json["job_id"] = 100;
    json["department_id"] = 10;
    json["manager_id"] = 5;
    json["first_name"] = "John";
    json["last_name"] = "Doe";
    json["hire_date"] = "2023-01-01";

    Person person(json);

    EXPECT_EQ(person.getValueOfId(), 1);
    EXPECT_EQ(person.getValueOfJobId(), 100);
    EXPECT_EQ(person.getValueOfDepartmentId(), 10);
    EXPECT_EQ(person.getValueOfManagerId(), 5);
    EXPECT_EQ(person.getValueOfFirstName(), "John");
    EXPECT_EQ(person.getValueOfLastName(), "Doe");
    EXPECT_EQ(person.getValueOfHireDate(), ::trantor::Date(2023, 1, 1));
}

TEST(PersonTest, ConstructorFromInvalidJson)
{
    Json::Value invalidJson;
    invalidJson["id"] = "invalid_id";

    EXPECT_THROW(Person person(invalidJson), Json::LogicError);
}

TEST(PersonTest, UpdateByJson)
{
    Json::Value json;
    json["id"] = 1;
    json["job_id"] = 100;
    json["department_id"] = 10;
    json["manager_id"] = 5;
    json["first_name"] = "John";
    json["last_name"] = "Doe";
    json["hire_date"] = "2023-01-01";

    Person person;
    person.updateByJson(json);

    EXPECT_EQ(person.getValueOfId(), 1);
    EXPECT_EQ(person.getValueOfJobId(), 100);
    EXPECT_EQ(person.getValueOfDepartmentId(), 10);
    EXPECT_EQ(person.getValueOfManagerId(), 5);
    EXPECT_EQ(person.getValueOfFirstName(), "John");
    EXPECT_EQ(person.getValueOfLastName(), "Doe");
    EXPECT_EQ(person.getValueOfHireDate(), ::trantor::Date(2023, 1, 1));
}

TEST(PersonTest, ValidateJsonForCreationSuccess)
{
    Json::Value json;
    json["job_id"] = 100;
    json["department_id"] = 10;
    json["manager_id"] = 5;
    json["first_name"] = "John";
    json["last_name"] = "Doe";
    json["hire_date"] = "2023-01-01";

    std::string error;
    EXPECT_TRUE(Person::validateJsonForCreation(json, error));
    EXPECT_TRUE(error.empty());
}

TEST(PersonTest, ValidateJsonForCreationFailure)
{
    Json::Value json;
    json["id"] = "invalid_id"; // Invalid type

    std::string error;
    EXPECT_FALSE(Person::validateJsonForCreation(json, error));
    EXPECT_FALSE(error.empty());
}

TEST(PersonTest, ToJson)
{
    Json::Value json;
    json["id"] = 1;
    json["job_id"] = 100;
    json["department_id"] = 10;
    json["manager_id"] = 5;
    json["first_name"] = "John";
    json["last_name"] = "Doe";
    json["hire_date"] = "2023-01-01";

    Person person(json);

    Json::Value toJsonResult = person.toJson();

    EXPECT_EQ(toJsonResult["id"].asInt(), 1);
    EXPECT_EQ(toJsonResult["job_id"].asInt(), 100);
    EXPECT_EQ(toJsonResult["department_id"].asInt(), 10);
    EXPECT_EQ(toJsonResult["manager_id"].asInt(), 5);
    EXPECT_EQ(toJsonResult["first_name"].asString(), "John");
    EXPECT_EQ(toJsonResult["last_name"].asString(), "Doe");
    EXPECT_EQ(toJsonResult["hire_date"].asString(), "2023-01-01");
}

TEST(PersonTest, ToMasqueradedJson)
{
    Json::Value json;
    json["id"] = 1;
    json["job_id"] = 100;
    json["department_id"] = 10;
    json["manager_id"] = 5;
    json["first_name"] = "John";
    json["last_name"] = "Doe";
    json["hire_date"] = "2023-01-01";

    Person person(json);

    std::vector<std::string> aliases = {"id_alias", "job_id_alias", "department_id_alias",
                                        "manager_id_alias", "first_name_alias", "last_name_alias", "hire_date_alias"};

    Json::Value masqueradedJson = person.toMasqueradedJson(aliases);

    EXPECT_EQ(masqueradedJson["id_alias"].asInt(), 1);
    EXPECT_EQ(masqueradedJson["job_id_alias"].asInt(), 100);
    EXPECT_EQ(masqueradedJson["department_id_alias"].asInt(), 10);
    EXPECT_EQ(masqueradedJson["manager_id_alias"].asInt(), 5);
    EXPECT_EQ(masqueradedJson["first_name_alias"].asString(), "John");
    EXPECT_EQ(masqueradedJson["last_name_alias"].asString(), "Doe");
    EXPECT_EQ(masqueradedJson["hire_date_alias"].asString(), "2023-01-01");
}
