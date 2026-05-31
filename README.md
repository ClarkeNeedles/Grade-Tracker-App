# Grade Tracker App (Qt + Supabase)

A desktop Course Grade and Productivity Application built using **C++ (Qt 6)** with a cloud-backed backend powered by **Supabase**. The system enables students to track courses, grades, study hours, and social ranking features in real time.

---

## 📌 Project Overview

This application is designed as a full-stack academic productivity system consisting of three core subsystems:

### 1. Graphical User Interface (GUI)
- Built using Qt Widgets
- Provides login, homepage, course pages, friend pages, and dialogs
- Acts as the primary user interaction layer

### 2. Database System
- Powered by Supabase (PostgreSQL backend)
- Handles user authentication, courses, grades, and social data
- Communicates via REST API using Qt Network module

### 3. Rank Computation System
- Computes user rankings based on course performance and study hours
- Updates dynamically based on database state

---

## 🧠 Key Features

- User authentication (login/register)
- Course tracking and grade management
- Study hour logging
- Friend system with comparative ranking
- Dynamic ranking system across courses
- Cloud-synced data via Supabase API

---

## 🏗️ Project Structure

```

src/        → Application source code (.cpp)
include/    → Header files (.h)
ui/         → Qt Designer UI files (.ui)
resources/  → Icons, assets, and QRC files

```

---

## 🔐 Environment Variables Setup

This project uses environment variables to store sensitive Supabase credentials.

### Step 1: Create your `.env` file

Copy the example file:

```

.env.example → .env

````

### Step 2: Fill in your credentials

```env
SUPABASE_BASE_URL=https://your-project.supabase.co/rest/v1
SUPABASE_AUTH_URL=https://your-project.supabase.co/auth/v1
SUPABASE_API_KEY=your-anon-key
````

### Step 3: Loading environment variables in Qt

Qt does not automatically load `.env` files. The project uses `QProcessEnvironment` or manual loading to access variables:

```cpp
QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

QString baseUrl = env.value("SUPABASE_BASE_URL");
QString authUrl = env.value("SUPABASE_AUTH_URL");
QByteArray apiKey = env.value("SUPABASE_API_KEY").toUtf8();
```

---

## ⚙️ Build Instructions

### Requirements

* Qt 6.x (Widgets + Network + Charts + SQL)
* CMake 3.16+
* MinGW or MSVC compiler

### Build Steps

```bash
git clone https://github.com/your-username/grade-tracker-app
cd grade-tracker-app
mkdir build
cd build
cmake ..
cmake --build .
```

---

## 📦 Required Qt Modules

* Qt Widgets
* Qt Network
* Qt Charts

---

## 🧩 System Design Summary

Based on the system design document, the application emphasizes:

* Strong separation between GUI, database, and computation layers
* Cloud-based persistent storage via Supabase
* Modular UI system supporting courses, friends, and rankings
* Sequential data processing with minimal concurrency complexity

---

## 🔒 Security Notes

* API keys are stored in environment variables (not hardcoded)
* Supabase handles authentication and encryption
* Network access is restricted through controlled REST queries

---

## 📈 Future Improvements

* Full career-wide ranking system
* Expanded friend comparison features
* Advanced analytics dashboards
* ROS-style modular backend refactor (optional exploration)

---
