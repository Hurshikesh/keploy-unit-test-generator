#include <gtest/gtest.h>
#include <drogon/drogon.h>
#include "../orgChartApi/plugins/JwtPlugin.h"
#include "../orgChartApi/plugins/Jwt.h"

class JwtPluginTest : public ::testing::Test {
protected:
    JwtPlugin plugin;
    Json::Value config;

    void SetUp() override {
        config["secret"] = "test_secret";
        config["sessionTime"] = 1234;
        config["issuer"] = "test_issuer";
        plugin.initAndStart(config);
    }
};

TEST_F(JwtPluginTest, InitAndStartStoresConfig) {
    // The config should be stored in the plugin (not directly accessible, but used in init())
    Jwt jwt = plugin.init();
    // If Jwt has public getters, check them:
    // EXPECT_EQ(jwt.getSecret(), "test_secret");
    // EXPECT_EQ(jwt.getSessionTime(), 1234);
    // EXPECT_EQ(jwt.getIssuer(), "test_issuer");

    // Instead, test encode/decode roundtrip (if Jwt doesn't have public getters)
    std::string token = jwt.encode("user_id", 42);
    auto decoded = jwt.decode(token);
    EXPECT_EQ(decoded.get_issuer(), "test_issuer");
    EXPECT_TRUE(decoded.has_payload_claim("user_id"));
    EXPECT_EQ(decoded.get_payload_claim("user_id").as_string(), "42");
}

TEST_F(JwtPluginTest, ShutdownDoesNotThrow) {
    EXPECT_NO_THROW({
        plugin.shutdown();
    });
}

TEST_F(JwtPluginTest, InitUsesDefaultConfigIfMissing) {
    JwtPlugin plugin2;
    Json::Value emptyConfig;
    plugin2.initAndStart(emptyConfig);
    Jwt jwt = plugin2.init();

    // The default issuer should be "auth0", sessionTime 3600, secret "secret"
    std::string token = jwt.encode("user_id", 7);
    auto decoded = jwt.decode(token);
    EXPECT_EQ(decoded.get_issuer(), "auth0");
    EXPECT_TRUE(decoded.has_payload_claim("user_id"));
    EXPECT_EQ(decoded.get_payload_claim("user_id").as_string(), "7");
}
