#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "tgbot.cpp"


TEST_CASE("Test addUser function") {
    // Create an in-memory database for testing
    sqlite3* db;
    sqlite3_open(":memory:", &db);

    // Create the necessary table
    const char* sql = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT, weight INTEGER, progress INTEGER);";
    char* err_msg = nullptr;
    sqlite3_exec(db, sql, nullptr, nullptr, &err_msg);

    // Create a test user
    User user = { 1, "John Doe", 80, 0 };
    addUser(db, user);

    // Verify that the user is added correctly
    User result = getUser(db, 1);
    CHECK_EQ(result.id, user.id);
    CHECK_EQ(result.name, user.name);
    CHECK_EQ(result.weight, user.weight);
    CHECK_EQ(result.progress, user.progress);

    // Close the database
    sqlite3_close(db);
}

TEST_CASE("Test updateUserWeight function") {
    // Create an in-memory database for testing
    sqlite3* db;
    sqlite3_open(":memory:", &db);

    // Create the necessary table
    const char* sql = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT, weight INTEGER, progress INTEGER);";
    char* err_msg = nullptr;
    sqlite3_exec(db, sql, nullptr, nullptr, &err_msg);

    // Create a test user
    User user = { 1, "John Doe", 80, 0 };
    addUser(db, user);

    // Update the user's weight
    user.weight = 75;
    updateUserWeight(db, user);

    // Verify that the user's weight is updated correctly
    User result = getUser(db, 1);
    CHECK_EQ(result.weight, user.weight);

    // Close the database
    sqlite3_close(db);
}

TEST_CASE("Test updateUserProgress function") {
    // Create an in-memory database for testing
    sqlite3* db;
    sqlite3_open(":memory:", &db);

    // Create the necessary table
    const char* sql = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT, weight INTEGER, progress INTEGER);";
    char* err_msg = nullptr;
    sqlite3_exec(db, sql, nullptr, nullptr, &err_msg);

    // Create a test user
    User user = { 1, "John Doe", 80, 0 };
    addUser(db, user);

    // Update the user's progress
    user.progress = 10;
    updateUserProgress(db, user);

    // Verify that the user's progress is updated correctly
    User result = getUser(db, 1);
    CHECK_EQ(result.progress, user.progress);

    // Close the database
    sqlite3_close(db);
}
