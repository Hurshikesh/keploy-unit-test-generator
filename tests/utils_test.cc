#include <gtest/gtest.h>
#include <drogon/HttpResponse.h>
#include <json/json.h>
#include <sstream>
#include <memory>
#include "../orgChartApi/utils/utils.h"

using namespace drogon;

// Helper to parse JSON from HttpResponse body
static Json::Value parseJsonBody(const HttpResponsePtr& response) {
    Json::CharReaderBuilder builder;
    Json::Value parsedContent;
    std::string errs;
    std::string content(response->getBody()); // Explicit conversion
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    bool ok = reader->parse(content.c_str(), content.c_str() + content.size(), &parsedContent, &errs);
    EXPECT_TRUE(ok) << "Failed to parse JSON: " << errs;
    return parsedContent;
}

TEST(UtilsTest, BadRequestValidInput) {
    bool callbackCalled = false;
    auto callback = [&](const HttpResponsePtr &response) {
        callbackCalled = true;
        ASSERT_TRUE(response != nullptr);
        EXPECT_EQ(response->getStatusCode(), k400BadRequest);

        auto parsedContent = parseJsonBody(response);
        EXPECT_EQ(parsedContent["error"].asString(), "Invalid Request");
    };

    badRequest(callback, "Invalid Request", k400BadRequest);
    EXPECT_TRUE(callbackCalled);
}

TEST(UtilsTest, BadRequestEmptyError) {
    bool callbackCalled = false;
    auto callback = [&](const HttpResponsePtr &response) {
        callbackCalled = true;
        ASSERT_TRUE(response != nullptr);
        EXPECT_EQ(response->getStatusCode(), k400BadRequest);

        auto parsedContent = parseJsonBody(response);
        EXPECT_EQ(parsedContent["error"].asString(), "");
    };

    badRequest(callback, "", k400BadRequest);
    EXPECT_TRUE(callbackCalled);
}

TEST(UtilsTest, MakeErrRespValidInput) {
    std::string errorMessage = "An error occurred";
    Json::Value result = makeErrResp(errorMessage);

    ASSERT_TRUE(result.isMember("error"));
    EXPECT_EQ(result["error"].asString(), errorMessage);
}

TEST(UtilsTest, MakeErrRespEmptyError) {
    std::string errorMessage = "";
    Json::Value result = makeErrResp(errorMessage);

    ASSERT_TRUE(result.isMember("error"));
    EXPECT_EQ(result["error"].asString(), errorMessage);
}

TEST(UtilsTest, BadRequestCustomStatusCode) {
    bool callbackCalled = false;
    auto callback = [&](const HttpResponsePtr &response) {
        callbackCalled = true;
        ASSERT_TRUE(response != nullptr);
        EXPECT_EQ(response->getStatusCode(), k403Forbidden);

        auto parsedContent = parseJsonBody(response);
        EXPECT_EQ(parsedContent["error"].asString(), "Forbidden error");
    };

    badRequest(callback, "Forbidden error", k403Forbidden);
    EXPECT_TRUE(callbackCalled);
}
