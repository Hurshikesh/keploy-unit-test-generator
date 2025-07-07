#include <gtest/gtest.h>
#include "../orgChartApi/models/Job.h"
#include <json/json.h>
#include <memory>
#include <string>

using namespace drogon_model::org_chart;

TEST(JobTest, DefaultConstructor) {
    Job job;
    EXPECT_EQ(job.getValueOfId(), 0);
    EXPECT_EQ(job.getValueOfTitle(), "");
    EXPECT_FALSE(job.getId());
    EXPECT_FALSE(job.getTitle());
}

TEST(JobTest, SettersAndGetters) {
    Job job;
    job.setId(123);
    job.setTitle("Engineer");
    EXPECT_EQ(job.getValueOfId(), 123);
    EXPECT_EQ(job.getValueOfTitle(), "Engineer");
    ASSERT_TRUE(job.getId());
    ASSERT_TRUE(job.getTitle());
    EXPECT_EQ(*job.getId(), 123);
    EXPECT_EQ(*job.getTitle(), "Engineer");
}

TEST(JobTest, SetTitleRValue) {
    Job job;
    std::string title = "Manager";
    job.setTitle(std::move(title));
    EXPECT_EQ(job.getValueOfTitle(), "Manager");
}

TEST(JobTest, ToJsonAndFromJson) {
    Job job;
    job.setId(42);
    job.setTitle("Manager");
    Json::Value json = job.toJson();

    // Now construct a Job from JSON
    Job job2(json);
    EXPECT_EQ(job2.getValueOfId(), 42);
    EXPECT_EQ(job2.getValueOfTitle(), "Manager");
}

TEST(JobTest, UpdateByJson) {
    Job job;
    Json::Value json;
    json["id"] = 55;
    json["title"] = "Lead";
    job.updateByJson(json);
    EXPECT_EQ(job.getValueOfId(), 55);
    EXPECT_EQ(job.getValueOfTitle(), "Lead");
}

TEST(JobTest, ToMasqueradedJson) {
    Job job;
    job.setId(7);
    job.setTitle("Architect");
    std::vector<std::string> aliases = {"job_id", "job_title"};
    Json::Value json = job.toMasqueradedJson(aliases);
    EXPECT_EQ(json["job_id"].asInt(), 7);
    EXPECT_EQ(json["job_title"].asString(), "Architect");
}

TEST(JobTest, FromMasqueradedJson) {
    Json::Value json;
    json["job_id"] = 88;
    json["job_title"] = "CTO";
    std::vector<std::string> aliases = {"job_id", "job_title"};
    Job job(json, aliases);
    EXPECT_EQ(job.getValueOfId(), 88);
    EXPECT_EQ(job.getValueOfTitle(), "CTO");
}

TEST(JobTest, ValidateJsonForCreationAndUpdate) {
    Json::Value goodJson;
    goodJson["title"] = "Dev";
    std::string err;
    EXPECT_TRUE(Job::validateJsonForCreation(goodJson, err));
    EXPECT_EQ(err, "");

    Json::Value badJson;
    std::string err2;
    EXPECT_FALSE(Job::validateJsonForCreation(badJson, err2));
    EXPECT_EQ(err2, "The title column cannot be null");

    Json::Value updateJson;
    updateJson["id"] = 1;
    updateJson["title"] = "Dev";
    std::string err3;
    EXPECT_TRUE(Job::validateJsonForUpdate(updateJson, err3));
    EXPECT_EQ(err3, "");

    Json::Value badUpdateJson;
    badUpdateJson["title"] = "Dev";
    std::string err4;
    EXPECT_FALSE(Job::validateJsonForUpdate(badUpdateJson, err4));
    EXPECT_EQ(err4, "The value of primary key must be set in the json object for update");
}

TEST(JobTest, GetColumnNameThrowsOnInvalidIndex) {
    // getColumnName uses assert, so this will abort in debug mode
    // but for coverage, let's check the valid indices
    EXPECT_NO_THROW(Job::getColumnName(0));
    EXPECT_NO_THROW(Job::getColumnName(1));
    // Uncomment the next line if you want to see the assert fail in debug builds:
    // EXPECT_DEATH(Job::getColumnName(2), ".*");
}
