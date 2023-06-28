#pragma once
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <stdio.h>
#include <tgbot/tgbot.h>
#include <vector>
#include <ctime>
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <set>

struct User {
    int id;
    std::string name;
    int weight;
    int progress;
};


/**
 * @brief Функция для добавления нового пользователя в базу данных.
 *
 * @param db Указатель на базу данных SQLite.
 * @param user Пользователь для добавления.
 */
void addUser(sqlite3* db, const User& user) {
    // Сформируйте SQL-запрос для добавления пользователя в базу данных
    std::string sql = "INSERT INTO users (id, name, weight, progress) VALUES (" + std::to_string(user.id) + ",'" + user.name + "', " + std::to_string(user.weight) + ", 0);";

    // Выполните SQL-запрос с помощью функции sqlite3_exec()
    char* err_msg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        printf("произошел казус!");
        std::cout << "error code: " << rc << std::endl;
        // Обработка ошибки выполнения запроса
        sqlite3_free(err_msg);
    }
}

/**
 * @brief Функция для получения пользователя из базы данных по заданному идентификатору.
 *
 * @param db Указатель на базу данных SQLite.
 * @param id Идентификатор пользователя.
 * @return Найденный пользователь.
 */
User getUser(sqlite3*db, int id) {
    std::string sql = "SELECT id, name, weight, progress FROM users WHERE id=" + std::to_string(id) + ";";
    std::vector<User> users;
    char* err_msg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), [](void* data, int argc, char** argv, char** col_name) -> int {
        // Обработка строки результата выборки
        User user = {
            atoi(argv[0]), // преобразуйте строку в целое число
            argv[1], // имя пользователя
            atoi(argv[2]), // преобразуйте строку в целое число
            atoi(argv[3]), // преобразуйте строку в целое число
        };
        // Добавьте пользователя в список
        static_cast<std::vector<User>*>(data)->push_back(user);
        return 0;
    }, &users, &err_msg);
    if (rc != SQLITE_OK) {
        // Обработка ошибки выполнения запроса
        sqlite3_free(err_msg);
    }

    return users[0];
}

/**
 * @brief Функция для обновления веса пользователя в базе данных.
 *
 * @param db Указатель на базу данных SQLite.
 * @param user Пользователь для обновления веса.
 */
void updateUserWeight(sqlite3* db, const User& user) {
    // Сформируйте SQL-запрос для добавления пользователя в базу данных
    std::string sql = "UPDATE users SET weight=" + std::to_string(user.weight) + " WHERE id=" + std::to_string(user.id) + ";";

    // Выполните SQL-запрос с помощью функции sqlite3_exec()
    char* err_msg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        printf("произошел казус!");
        std::cout << "error code: " << rc << std::endl;
        // Обработка ошибки выполнения запроса
        sqlite3_free(err_msg);
    }
}

/**
 * @brief Функция для обновления прогресса пользователя в базе данных.
 *
 * @param db Указатель на базу данных SQLite.
 * @param user Пользователь для обновления прогресса.
 */
void updateUserProgress(sqlite3* db, const User& user) {
    // Сформируйте SQL-запрос для добавления пользователя в базу данных
    std::string sql = "UPDATE users SET progress=" + std::to_string(user.progress) + " WHERE id=" + std::to_string(user.id) + ";";

    // Выполните SQL-запрос с помощью функции sqlite3_exec()
    char* err_msg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        printf("произошел казус!");
        std::cout << "error code: " << rc << std::endl;
        // Обработка ошибки выполнения запроса
        sqlite3_free(err_msg);
    }
}

/**
 * @brief Функция для получения списка всех пользователей из базы данных
 *
 * @param db Указатель на базу данных SQLite.
 * @return Вектор из всех пользователей
 */
std::vector<User> getAllUsers(sqlite3* db) {
    std::vector<User> users;

    // Сформируйте SQL-запрос для выборки всех строк из таблицы users
    std::string sql = "SELECT id, name, weight, progress FROM users;";

    // Выполните SQL-запрос с помощью функции sqlite3_exec()
    char* err_msg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), [](void* data, int argc, char** argv, char** col_name) -> int {
        // Обработка строки результата выборки
        User user = {
            atoi(argv[0]), // преобразуйте строку в целое число
            argv[1], // имя пользователя
            atoi(argv[2]), // преобразуйте строку в целое число
            atoi(argv[3]), // преобразуйте строку в целое число
        };
        // Добавьте пользователя в список
        static_cast<std::vector<User>*>(data)->push_back(user);
        return 0;
    }, &users, &err_msg);
    if (rc != SQLITE_OK) {
        // Обработка ошибки выполнения запроса
        sqlite3_free(err_msg);
    }

    return users;
}

bool is_number(const std::string &s) {
  return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

//         g++ main.cpp -o main --std=c++14 -I/usr/local/include -lTgBot -lboost_system -lssl -lcrypto -lpthread -lcurl -lsqlite3
//         g++ tests.cpp -o tests --std=c++14 -I/usr/local/include -lTgBot -lboost_system -lssl -lcrypto -lpthread -lcurl -lsqlite3

//         ./main
