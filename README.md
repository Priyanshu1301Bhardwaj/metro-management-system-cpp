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

### Compile

```bash
g++ src/main.cpp -o metro
```

### Run

```bash
./metro
```

---

## 📂 Sample Input

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

## 📌 Future Improvements

* GUI (Qt / Web)
* Real metro data integration
* Dijkstra (weighted graph)
* Online payment integration

---

## 👨‍💻 Author

Priyanshu Bhardwaj

---

## ⭐ If you like this project, give it a star!
