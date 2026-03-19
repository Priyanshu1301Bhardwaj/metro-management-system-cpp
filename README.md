# 🚇 Metro Management System (C++)

## 📌 Overview

This project is a **C++ based Metro Management System** that simulates real-world metro operations including:

* Shortest route calculation (BFS)
* Ticket booking system
* User authentication (signup/login)
* Fare calculation with peak/off-peak pricing
* Trip history tracking
* Sudoku solver (bonus feature)

---

## 🛠️ Technologies Used

* C++
* STL (Standard Template Library)
* File Handling
* Graphs (Adjacency List)
* BFS Algorithm
* OOP Concepts

---

## ✨ Features

### 🚉 Metro System

* Add metro connections dynamically
* View all stations
* Display adjacency list
* Find shortest path between stations

### 🎟️ Ticket Booking

* Calculates fare based on:

  * Number of stations
  * Peak / Off-peak hours
* Saves trip history

### 🔐 User System

* Signup & Login
* Password hashing
* Persistent storage (`users.db`)

### 🧠 Algorithms Used

* **BFS (Breadth First Search)** → shortest path
* **Backtracking** → Sudoku solver

### 🎮 Bonus

* Built-in Sudoku solver

---

## ⏱️ Peak Hours Logic

* Morning: 7 AM – 10 AM
* Evening: 5 PM – 8 PM
* Fare multiplier: **1.5x**

---

## ▶️ How to Run

### ⚠️ Requirements

* C++ compiler (g++) installed

---

### 📥 1. Clone the Repository

```bash
git clone https://github.com/Priyanshu1301Bhardwaj/metro-management-system-cpp.git
cd metro-management-system-cpp
```

---

### 📁 2. Ensure `data/` Folder Exists

The project stores user data in `data/users.db`.

If the folder is missing, create it:

```bash
mkdir data
```

---

### 🔧 3. Compile the Program

```bash
g++ src/main.cpp -o metro
```

---

### ▶️ 4. Run the Program

#### 🐧 Linux / 🍎 macOS

```bash
./metro
```

#### 🪟 Windows

```bash
metro.exe
```

---

### ⚙️ 5. First Run

* The program will create `users.db` automatically inside the `data/` folder
* You may see:

```
No existing user data found. Starting fresh.
```

---

### 📌 6. Provide Input

Enter metro connections when prompted.

Example:

```
6
noidacity golfcourse
noidacity mayurvihar
mayurvihar golfcourse
mayurvihar dwarka
dwarka rajivchok
sector62 noidacity
```

---

### 🎮 7. Use the System

* Signup / Login
* Book tickets
* Check shortest routes
* View history
* Play Sudoku

---


## 📌 Future Improvements

* GUI (Qt / Web)
* Real metro data integration
* Dijkstra (weighted graph)
* Online payment integration
