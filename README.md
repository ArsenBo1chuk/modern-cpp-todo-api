# ⚡ Modern To-Do REST API Server

![C++](https://img.shields.io/badge/C%2B%2B-20-blue?style=for-the-badge)
![Crow](https://img.shields.io/badge/Crow-HTTP_Framework-success?style=for-the-badge)
![SQLite](https://img.shields.io/badge/SQLite-Database-orange?style=for-the-badge)
![REST API](https://img.shields.io/badge/REST-API-red?style=for-the-badge)

A lightweight RESTful To-Do API built with **C++20**, **Crow** and **SQLite3**.

The project demonstrates modern backend development practices including:

- REST architecture
- Thread-safe database access
- JSON request/response handling
- SQLite persistence
- Multithreaded HTTP server

---

## ✨ Features

### 📋 Task Management

- Create tasks
- Retrieve all tasks
- Mark tasks as completed
- Delete tasks

### 🚀 Backend Features

- SQLite database storage
- Automatic database initialization
- Multithreaded HTTP server
- Thread-safe database operations using `std::mutex`
- JSON API responses
- Prepared SQL statements
- RESTful design

---

## 🏗 Architecture

```text
Client
 │
 ▼
HTTP Request
 │
 ▼
Crow Web Server
 │
 ▼
Route Handler
 │
 ▼
SQLite Database
```

---

## 📡 API Endpoints

### Get All Tasks

```http
GET /tasks
```

Response:

```json
{
  "tasks": [
    {
      "id": 1,
      "title": "Learn Crow",
      "is_completed": false
    }
  ]
}
```

---

### Create Task

```http
POST /tasks
```

Request:

```json
{
  "title": "Learn SQLite"
}
```

Response:

```json
{
  "id": 1,
  "title": "Learn SQLite",
  "is_completed": false
}
```

---

### Update Task Status

```http
PUT /tasks/{id}
```

Request:

```json
{
  "is_completed": true
}
```

---

### Delete Task

```http
DELETE /tasks/{id}
```

---

## 📂 Project Structure

```text
.
├── build/
├── CMakeLists.txt
├── main.cpp
├── requests.http
└── README.md
```

---

## 🛠 Technologies

| Technology | Purpose |
|------------|----------|
| C++20 | Core language |
| Crow | HTTP framework |
| SQLite3 | Database |
| CMake | Build system |
| JSON | API communication |

---

## ⚙️ Build

### Configure

```bash
cmake -B build -S .
```

### Compile

```bash
cmake --build build
```

---

## ▶️ Run

### Linux

```bash
./build/todo_server
```

### Windows

```powershell
.\build\todo_server.exe
```

Server starts on:

```text
http://localhost:18080
```

---

## 🧪 Testing

### Create Task

```bash
curl -X POST http://localhost:18080/tasks ^
-H "Content-Type: application/json" ^
-d "{\"title\":\"Learn C++\"}"
```

### Get Tasks

```bash
curl http://localhost:18080/tasks
```

### Update Task

```bash
curl -X PUT http://localhost:18080/tasks/1 ^
-H "Content-Type: application/json" ^
-d "{\"is_completed\":true}"
```

### Delete Task

```bash
curl -X DELETE http://localhost:18080/tasks/1
```

---

## 🔒 Thread Safety

Database operations are protected using:

```cpp
std::mutex
std::lock_guard<std::mutex>
```

This prevents concurrent write conflicts when multiple clients access the API simultaneously.

---

## 📈 Future Improvements

- [ ] User authentication
- [ ] Task categories
- [ ] Due dates
- [ ] PostgreSQL support
- [ ] Docker deployment
- [ ] Unit testing
- [ ] JWT authorization
- [ ] Swagger/OpenAPI documentation

---

## 👨‍💻 Author

**Arsen** — Backend Developer (C++)

[![GitHub](https://img.shields.io/badge/GitHub-181717?style=for-the-badge&logo=github&logoColor=white)](https://github.com/ArsenBo1chuk)
[![Telegram](https://img.shields.io/badge/Telegram-26A5E4?style=for-the-badge&logo=telegram&logoColor=white)](https://t.me/Arsen_Bo1chuk)

---

## ⭐ Support

If you found this project useful, consider giving it a star.