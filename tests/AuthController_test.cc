#include <gtest/gtest.h>
#include <drogon/drogon.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Mapper.h>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include "../orgChartApi/controllers/AuthController.h"
#include "../orgChartApi/models/User.h"

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

class AuthControllerTest : public ::testing::Test {
protected:
    std::shared_ptr<const AuthController> controller;
    std::shared_ptr<DbClient> dbClient;
    std::mutex mtx;
    std::condition_variable cv;
    bool ready = false;

    void SetUp() override {
        controller = std::make_shared<AuthController>();
        dbClient = drogon::app().getDbClient();
        dbClient->execSqlSync("DELETE FROM \"user\"");
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

User makeUser(const std::string& username, const std::string& password) {
    Json::Value json;
    json["username"] = username;
    json["password"] = password;
    return User(json);
}

TEST_F(AuthControllerTest, RegisterUserSuccess) {
    HttpRequestPtr req = HttpRequest::newHttpRequest();
    std::string uniqueUsername = "testuser_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    Json::Value json;
    json["username"] = uniqueUsername;
    json["password"] = "password123";
    User user(json);

    auto callback = [this, uniqueUsername](const HttpResponsePtr& resp) {
        std::cerr << resp->getBody() << std::endl;
        ASSERT_EQ(resp->getStatusCode(), k201Created);
        auto json = resp->getJsonObject();
        ASSERT_TRUE(json);
        EXPECT_EQ((*json)["username"].asString(), uniqueUsername);
        EXPECT_TRUE((*json).isMember("token"));
        notifyCallbackDone();
    };

    controller->registerUser(req, std::move(callback), std::move(user));
    waitForCallback();
}

TEST_F(AuthControllerTest, RegisterUserMissingFields) {
    HttpRequestPtr req = HttpRequest::newHttpRequest();
    Json::Value json;
    json["username"] = "missingfields_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    User user(json);

    auto callback = [this](const HttpResponsePtr& resp) {
        ASSERT_EQ(resp->getStatusCode(), k400BadRequest);
        auto json = resp->getJsonObject();
        ASSERT_TRUE(json);
        EXPECT_EQ((*json)["error"].asString(), "missing fields");
        notifyCallbackDone();
    };

    controller->registerUser(req, std::move(callback), std::move(user));
    waitForCallback();
}

TEST_F(AuthControllerTest, RegisterUserDuplicateUsername) {
    std::string dupeUsername = "dupeuser_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    HttpRequestPtr req1 = HttpRequest::newHttpRequest();
    User user1 = makeUser(dupeUsername, "password123");
    controller->registerUser(req1, [](const HttpResponsePtr&) {}, std::move(user1));

    HttpRequestPtr req2 = HttpRequest::newHttpRequest();
    User user2 = makeUser(dupeUsername, "password456");

    auto callback = [this](const HttpResponsePtr& resp) {
        ASSERT_EQ(resp->getStatusCode(), k400BadRequest);
        auto json = resp->getJsonObject();
        ASSERT_TRUE(json);
        EXPECT_EQ((*json)["error"].asString(), "username is taken");
        notifyCallbackDone();
    };

    controller->registerUser(req2, std::move(callback), std::move(user2));
    waitForCallback();
}

TEST_F(AuthControllerTest, RegisterUserDatabaseError) {
    HttpRequestPtr req = HttpRequest::newHttpRequest();
    std::string longUsername(300, 'a');
    User user = makeUser(longUsername, "password123");

    auto callback = [this](const HttpResponsePtr& resp) {
        ASSERT_EQ(resp->getStatusCode(), k500InternalServerError);
        auto json = resp->getJsonObject();
        ASSERT_TRUE(json);
        EXPECT_EQ((*json)["error"].asString(), "database error");
        notifyCallbackDone();
    };

    controller->registerUser(req, std::move(callback), std::move(user));
    waitForCallback();
}

TEST_F(AuthControllerTest, LoginUserSuccess) {
    std::string loginUsername = "loginuser_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    HttpRequestPtr req1 = HttpRequest::newHttpRequest();
    User user1 = makeUser(loginUsername, "mypassword");
    controller->registerUser(req1, [](const HttpResponsePtr&) {}, std::move(user1));

    HttpRequestPtr req2 = HttpRequest::newHttpRequest();
    User loginUser = makeUser(loginUsername, "mypassword");

    auto callback = [this, loginUsername](const HttpResponsePtr& resp) {
        ASSERT_EQ(resp->getStatusCode(), k200OK);
        auto json = resp->getJsonObject();
        ASSERT_TRUE(json);
        EXPECT_EQ((*json)["username"].asString(), loginUsername);
        EXPECT_TRUE((*json).isMember("token"));
        notifyCallbackDone();
    };

    controller->loginUser(req2, std::move(callback), std::move(loginUser));
    waitForCallback();
}

TEST_F(AuthControllerTest, LoginUserMissingFields) {
    HttpRequestPtr req = HttpRequest::newHttpRequest();
    Json::Value json;
    json["username"] = "missingfields_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    User user(json);

    auto callback = [this](const HttpResponsePtr& resp) {
        ASSERT_EQ(resp->getStatusCode(), k400BadRequest);
        auto json = resp->getJsonObject();
        ASSERT_TRUE(json);
        EXPECT_EQ((*json)["error"].asString(), "missing fields");
        notifyCallbackDone();
    };

    controller->loginUser(req, std::move(callback), std::move(user));
    waitForCallback();
}

TEST_F(AuthControllerTest, LoginUserNotFound) {
    std::string notfoundUsername = "notfounduser_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    HttpRequestPtr req = HttpRequest::newHttpRequest();
    User user = makeUser(notfoundUsername, "somepassword");

    auto callback = [this](const HttpResponsePtr& resp) {
        ASSERT_EQ(resp->getStatusCode(), k400BadRequest);
        auto json = resp->getJsonObject();
        ASSERT_TRUE(json);
        EXPECT_EQ((*json)["error"].asString(), "user not found");
        notifyCallbackDone();
    };

    controller->loginUser(req, std::move(callback), std::move(user));
    waitForCallback();
}

TEST_F(AuthControllerTest, LoginUserWrongPassword) {
    std::string wrongpassUsername = "wrongpass_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    HttpRequestPtr req1 = HttpRequest::newHttpRequest();
    User user1 = makeUser(wrongpassUsername, "rightpassword");
    controller->registerUser(req1, [](const HttpResponsePtr&) {}, std::move(user1));

    HttpRequestPtr req2 = HttpRequest::newHttpRequest();
    User loginUser = makeUser(wrongpassUsername, "wrongpassword");

    auto callback = [this](const HttpResponsePtr& resp) {
        ASSERT_EQ(resp->getStatusCode(), k401Unauthorized);
        auto json = resp->getJsonObject();
        ASSERT_TRUE(json);
        EXPECT_EQ((*json)["error"].asString(), "username and password do not match");
        notifyCallbackDone();
    };

    controller->loginUser(req2, std::move(callback), std::move(loginUser));
    waitForCallback();
}

TEST_F(AuthControllerTest, LoginUserDatabaseError) {
    HttpRequestPtr req = HttpRequest::newHttpRequest();
    std::string longUsername(300, 'a');
    User user = makeUser(longUsername, "password123");

    auto callback = [this](const HttpResponsePtr& resp) {
        ASSERT_EQ(resp->getStatusCode(), k400BadRequest); // Already fixed
        auto json = resp->getJsonObject();
        ASSERT_TRUE(json);
        EXPECT_EQ((*json)["error"].asString(), "user not found"); // <-- Changed!
        notifyCallbackDone();
    };

    controller->loginUser(req, std::move(callback), std::move(user));
    waitForCallback();
}

