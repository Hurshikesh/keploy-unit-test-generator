#include <gtest/gtest.h>
#include "../orgChartApi/plugins/Jwt.h"
#include <jwt-cpp/jwt.h>
#include <string>
#include <thread>

class JwtTest : public ::testing::Test {
protected:
    std::string secret = "test_secret";
    int sessionTime = 3600; // 1 hour
    std::string issuer = "test_issuer";
    Jwt jwt{secret, sessionTime, issuer};
};

TEST_F(JwtTest, EncodeAndDecodeValidToken) {
    std::string field = "user_id";
    int value = 42;
    std::string token = jwt.encode(field, value);

    // Token should not be empty
    ASSERT_FALSE(token.empty());

    // Decode and verify
    auto decoded = jwt.decode(token);
    // Check issuer
    EXPECT_EQ(decoded.get_issuer(), issuer);
    // Check claim
    ASSERT_TRUE(decoded.has_payload_claim(field));
    EXPECT_EQ(decoded.get_payload_claim(field).as_string(), std::to_string(value));
}

TEST_F(JwtTest, TokenHasCorrectExpiration) {
    std::string field = "user_id";
    int value = 99;
    std::string token = jwt.encode(field, value);

    auto decoded = jwt.decode(token);
    auto now = std::chrono::system_clock::now();
    auto exp = decoded.get_expires_at();
    auto iat = decoded.get_issued_at();

    // Should expire in about sessionTime seconds
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(exp - iat).count();
    EXPECT_GE(duration, sessionTime - 1); // Allow for rounding
    EXPECT_LE(duration, sessionTime + 1);
}

TEST_F(JwtTest, DecodeWithWrongSecretThrows) {
    std::string field = "user_id";
    int value = 123;
    std::string token = jwt.encode(field, value);

    Jwt jwtWrongSecret{"wrong_secret", sessionTime, issuer};
    EXPECT_THROW({
        jwtWrongSecret.decode(token);
    }, std::exception);
}

TEST_F(JwtTest, DecodeWithWrongIssuerThrows) {
    std::string field = "user_id";
    int value = 123;
    std::string token = jwt.encode(field, value);

    Jwt jwtWrongIssuer{secret, sessionTime, "other_issuer"};
    EXPECT_THROW({
        jwtWrongIssuer.decode(token);
    }, std::exception);
}

TEST_F(JwtTest, DecodeExpiredTokenThrows) {
    // Use a very short session time
    Jwt shortJwt{secret, 1, issuer};
    std::string field = "user_id";
    int value = 555;
    std::string token = shortJwt.encode(field, value);

    // Wait for the token to expire
    std::this_thread::sleep_for(std::chrono::seconds(2));

    EXPECT_THROW({
        shortJwt.decode(token);
    }, std::exception);
}
