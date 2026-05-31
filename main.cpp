#include <iostream>
#include <string>
#include <sqlite3.h>
#include "crow.h"
#include <mutex>
#undef DELETE

std::mutex db_mutex;

bool init_db()
{
    sqlite3 *DB;
    int rc = sqlite3_open("todo.db", &DB);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Не вдалося відкрити БД." << sqlite3_errmsg(DB) << std::endl;
        return false;
    }

    std::string sql = "CREATE TABLE IF NOT EXISTS tasks("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "title TEXT NOT NULL,"
                      "is_completed INTEGER DEFAULT 0);";

    char *errormsg = nullptr;
    rc = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &errormsg);
    if (rc != SQLITE_OK)
    {
        sqlite3_free(errormsg);
        sqlite3_close(DB);
        return false;
    }

    sqlite3_close(DB);
    return true;
}

int main()
{
    if (!init_db())
    {
        return 1;
    }

    crow::SimpleApp to_do;

    CROW_ROUTE(to_do, "/tasks")([]()
                                {
        std::lock_guard<std::mutex> lock(db_mutex);
        sqlite3*DB;
        sqlite3_open("todo.db", &DB);

        std::string sql = "SELECT id, title, is_completed FROM tasks;";
        sqlite3_stmt*stmt;

        crow::json::wvalue::list tasks_list;

        if(sqlite3_prepare_v2(DB, sql.c_str(),-1, &stmt, nullptr) == SQLITE_OK)
        {
            while(sqlite3_step(stmt) == SQLITE_ROW)
            {
                crow::json::wvalue task;
                task["id"] = sqlite3_column_int(stmt, 0);
                const unsigned char*text_ptr = sqlite3_column_text(stmt, 1);
                std::string title_str = text_ptr ? reinterpret_cast<const char*>(text_ptr) : "";
                task["title"] = title_str;
                task["is_completed"] = sqlite3_column_int(stmt, 2) != 0;
                tasks_list.push_back(task);
            }
        }
        sqlite3_finalize(stmt);
        sqlite3_close(DB);

        crow::json::wvalue response;
        response["tasks"] = std::move(tasks_list);
        return crow::response(200, response); });

    CROW_ROUTE(to_do, "/tasks").methods(crow::HTTPMethod::POST)([](const crow::request &req)
                                                                {
        auto body = crow::json::load(req.body);
        if(!body || !body.has("title"))
        {
            return crow::response(400, "Missing 'title'");
        }

        std::string title = body["title"].s();

        std::lock_guard<std::mutex> lock(db_mutex);
        sqlite3*DB;
        sqlite3_open("todo.db", &DB);

        std::string sql = "INSERT INTO tasks(title, is_completed) VALUES(?, 0);";
        int task_id = -1;
        sqlite3_stmt*stmt;

        if(sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr)== SQLITE_OK)
        {
            sqlite3_bind_text(stmt, 1, title.c_str(), -1 , SQLITE_STATIC);
            if(sqlite3_step(stmt) == SQLITE_DONE)
            task_id = sqlite3_last_insert_rowid(DB);
        }
        sqlite3_finalize(stmt);
        sqlite3_close(DB);

        if(task_id == -1)
        {
            return crow::response(500, "Database error");
        }

        crow::json::wvalue response;
        response["id"] = task_id;
        response["title"] = title;
        response["is_completed"] = false;
        return crow::response(201, response); });

    CROW_ROUTE(to_do, "/tasks/<int>").methods(crow::HTTPMethod::PUT)([](const crow::request &req, int id)
                                                                     {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("is_completed")) {
            return crow::response(400, "Некоректний JSON або відсутнє поле 'is_completed'");
        }

        int is_completed =body["is_completed"].b() ? 1 : 0;
        std::lock_guard<std::mutex> lock(db_mutex);
        sqlite3*DB;
        sqlite3_open("todo.db", &DB);
        std::string sql = "UPDATE tasks SET is_completed = ? WHERE id = ?;";
        sqlite3_stmt*stmt;
        bool success = false;
        
        if(sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            sqlite3_bind_int(stmt, 2, id);
            sqlite3_bind_int(stmt, 1, is_completed);
            if(sqlite3_step(stmt) == SQLITE_DONE)
            {
                if(sqlite3_changes(DB)>0)
                {
                    success = true;
                }
            }

        }

        sqlite3_finalize(stmt);
        sqlite3_close(DB);

        if(!success)
        {
            return crow::response(400, "Not found");
        }

        crow::json::wvalue response;
        response["status"] = "success";
        return crow::response(200, response); });

    CROW_ROUTE(to_do, "/tasks/<int>").methods(crow::HTTPMethod::DELETE)([](int id)
                                                                        {
        std::lock_guard<std::mutex> lock(db_mutex);
        sqlite3*DB;
        sqlite3_open("todo.db", &DB);
        std::string sql = "DELETE FROM tasks WHERE id = ?;";
        sqlite3_stmt*stmt;
        bool success = false;
        if(sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            sqlite3_bind_int(stmt, 1, id);
            if(sqlite3_step(stmt) == SQLITE_DONE)
            {
                if(sqlite3_changes(DB)>0)
                {
                    success = true;
                }
            }
        }

        sqlite3_finalize(stmt);
        sqlite3_close(DB);

        if(!success)
        {
            return crow::response(400, "Not found");
        }

        crow::json::wvalue response;
        response["status"] = "deleted";
        return crow::response(200, response); });

    to_do.port(18080).multithreaded().run();
}