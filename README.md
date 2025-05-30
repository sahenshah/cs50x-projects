# 📘 CS50x Projects

This repository contains my solutions to the problem sets from [CS50x](https://cs50.harvard.edu/x/), Harvard University's Introduction to Computer Science course. The projects span multiple languages and domains, including C, Python, SQL, HTML/CSS/JavaScript, and Flask.

---

## 🧠 Table of Contents

* [C](#-c)
* [Arrays](#-arrays)
* [Algorithms](#-algorithms)
* [Memory](#-memory)
* [Data Structures](#-data-structures)
* [Python](#-python)
* [SQL](#-sql)
* [Web: HTML, CSS & JS](#-html-css-and-javascript)
* [Flask Web Applications](#-flask)

---

## 🧮 C

| Project          | Description                                               |
| ---------------- | --------------------------------------------------------- |
| **World**        | Basic "Hello, World!" program.                            |
| **Me**           | Greets the user by name.                                  |
| **Mario-more**   | Prints Super Mario-style pyramids with adjustable height. |
| **Credit**       | Validates credit card numbers using Luhn’s Algorithm.     |

---

## 🗂️ Arrays

| Project          | Description                                                     |
| ---------------- | --------------------------------------------------------------- |
| **Scrabble**     | Simulates a two-player Scrabble game and determines the winner. |
| **Readability**  | Calculates the reading grade level of input text.               |
| **Substitution** | Encrypts text using a user-supplied substitution cipher.        |

---

## ⚙️ Algorithms

| Project       | Description                                                    |
| ------------- | -------------------------------------------------------------- |
| **Sort**      | Analysis and comparison of selection, bubble, and merge sorts. |
| **Plurality** | Simulates a plurality election (first-past-the-post).          |
| **Tideman**   | Simulates an election using the Tideman (ranked pairs) method. |

---

## 🧬 Memory

| Project     | Description                                                              |
| ----------- | ------------------------------------------------------------------------ |
| **malloc**  | Memory allocation practice.                                              |
| **Volume**  | Adjusts the volume of an audio file (WAV) by a given factor.             |
| **Filter**  | Applies grayscale, blur, reflection, and edge detection filters to BMPs. |
| **Recover** | Recovers JPEG images from a raw forensic memory image.                   |

---

## 🌳 Data Structures

| Project         | Description                                                                  |
| --------------- | ---------------------------------------------------------------------------- |
| **Inheritance** | Simulates blood type inheritance using structs and randomization.            |
| **Speller**     | Spell-checker that uses a hash table to validate words against a dictionary. |
| *Concepts*      | Implementation of linked lists, binary trees, and hash tables (chaining).    |

---

## 🐍 Python

Re-implementations of some earlier C projects in Python:

* Hello, Me, Mario-more, Credit, Readability
* **DNA**: Matches DNA sequences to individuals based on STR counts in a CSV file.

---

## 🗃️ SQL

### 📀 Songs

SQL queries that explore a music database:

* List songs, sort by tempo, find top 5, filter by danceability, artist-specific queries (Post Malone, Drake), etc.

### 🎮 Movies

SQL queries exploring an IMDb-style database:

* Filter by year, actor, rating, director, Harry Potter films, co-stars, and more.

### 🕵️ Fiftyville

Solve a mystery using SQL queries:

* Use provided `log.sql` and a forensic database to uncover:

  * The thief
  * The escape city
  * The accomplice
* Use `fiftyville.py` to automate and solve based on SQL insights.

---

## 🌐 HTML, CSS, and JavaScript

| Project      | Description                                                                                                                                         |
| ------------ | --------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Trivia**   | A webpage quiz with multiple-choice questions.                                                                                                      |
| **Homepage** | A four-page website using HTML, CSS, Bootstrap, and JavaScript.                                                                                     |
| *Resources*  | W3Schools used as a reference: [HTML](https://www.w3schools.com/html/), [CSS](https://www.w3schools.com/css/), [JS](https://www.w3schools.com/js/). |

---

## 🌍 Flask

| Project       | Description                                              |
| ------------- | -------------------------------------------------------- |
| **Birthdays** | Flask app to track user birthdays using an SQL database. |
| **Finance**   | Full-stack stock trading simulation with:                |

* Registration & login
* Stock quote lookups via CS50 API
* SQL database integration
* Jinja template rendering |

---

## 🏑 How to Run

For C or Python projects:

```bash
make project_name  # For C files
./project_name     # Run the compiled binary

python3 project.py # For Python files
```

For web apps (HTML/Flask):

```bash
http-server       # For frontend-only projects
flask run         # For Flask apps
```

---

## 📜 License

These projects are for educational purposes and follow CS50’s [academic honesty policy](https://cs50.harvard.edu/x/honesty/).

---
