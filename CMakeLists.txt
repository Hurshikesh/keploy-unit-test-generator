cmake_minimum_required(VERSION 3.14)

set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS TRUE CACHE INTERNAL "Suppress developer warnings")

project(UnitTestGenerator)

option(CODE_COVERAGE "Enable coverage reporting" ON)
if(CODE_COVERAGE AND (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU"))
  message(STATUS "Building with code coverage flags")
  add_compile_options(--coverage -O0 -g)
  add_link_options(--coverage)
endif()

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/orgChartApi/models
  ${CMAKE_CURRENT_SOURCE_DIR}/orgChartApi/controllers
  ${CMAKE_SOURCE_DIR}/third_party/jwt-cpp/include
  ${CMAKE_SOURCE_DIR}/third_party/libbcrypt/include
  ${CMAKE_SOURCE_DIR}/orgChartApi
  ${CMAKE_SOURCE_DIR}/orgChartApi/third_party/jwt-cpp/include
)

add_executable(AllTests
  tests/Department_test.cc
  tests/DepartmentsController_test.cc
  tests/Job_test.cc
  tests/LoginFilter.cc
  tests/Person_test.cc
  tests/PersonInfo_test.cc
  tests/test_main.cc
  tests/test_shim.cc
  tests/User_test.cc
  tests/utils_test.cc
  tests/AuthController_test.cc
  tests/JobsController_test.cc
  tests/PersonsController_test.cc
  tests/Jwt_test.cc
  tests/JwtPlugin_test.cc
  tests/LoginFilter_test.cc

  orgChartApi/models/Department.cc
  orgChartApi/models/Person.cc
  orgChartApi/models/Job.cc
  orgChartApi/models/User.cc
  orgChartApi/models/PersonInfo.cc
  orgChartApi/controllers/DepartmentsController.cc
  orgChartApi/controllers/AuthController.cc
  orgChartApi/controllers/JobsController.cc
  orgChartApi/controllers/PersonsController.cc
  orgChartApi/utils/utils.cc
  orgChartApi/plugins/Jwt.cc
  orgChartApi/plugins/JwtPlugin.cc
  orgChartApi/filters/LoginFilter.cc
)

find_package(Drogon REQUIRED)
link_directories(/usr/local/lib)

target_include_directories(AllTests PRIVATE
  ${CMAKE_SOURCE_DIR}/orgChartApi
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/orgChartApi/models
  ${CMAKE_CURRENT_SOURCE_DIR}/orgChartApi/controllers
  ${CMAKE_SOURCE_DIR}/orgChartApi/third_party/jwt_cpp/include
  ${CMAKE_SOURCE_DIR}/orgChartApi/third_party/jwt_cpp/include/jwt-cpp
  ${CMAKE_SOURCE_DIR}/third_party/libbcrypt/include
  ${CMAKE_SOURCE_DIR}/orgChartApi/third_party/jwt-cpp/include
)

target_link_libraries(AllTests PRIVATE bcrypt
  gtest
  Drogon::Drogon
  jsoncpp
  /usr/local/lib/libdrogon.a
)

include(GoogleTest)
gtest_discover_tests(AllTests)

add_custom_command(TARGET AllTests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/config.json $<TARGET_FILE_DIR:AllTests>)
