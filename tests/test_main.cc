#include <gtest/gtest.h>
#include <drogon/drogon.h>
#include "LoginFilter.h"
#include <thread>
#include <chrono>

// Force the linker to keep LoginFilter in the binary
namespace {
    [[maybe_unused]]
    void* force_link_login_filter = (void*)new LoginFilter();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    // Load Drogon config (make sure config.json is in build/)
    drogon::app().loadConfigFile("config.json");

    // Start Drogon app in background
    std::thread([] {
        drogon::app().run();
    }).detach();

    std::this_thread::sleep_for(std::chrono::seconds(2)); // Give Drogon time to start

    return RUN_ALL_TESTS();
}
