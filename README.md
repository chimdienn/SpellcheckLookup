# 📚 Spellcheck Dictionary using Linked List & Patricia Tree

This project was developed as part of **COMP20003 Algorithms and Data Structures (Semester 2, 2024)** at the **University of Melbourne**.  
It implements a **dictionary spellcheck system** in **C** using both **Linked List** and **Patricia Tree** data structures, then compares their efficiency through theoretical and empirical analysis.  

---

## ✨ Features

- ✅ Dictionary built from real-world suburb dataset (CSV format).  
- 🔎 Query system with three outcomes:
  - **Exact match** → outputs all matching records.  
  - **Similar match** (Patricia tree only) → outputs closest key recommendation.  
  - **No match** → indicates no match or recommendation.  
- 📊 Comparison counts (string comparisons, bit comparisons, node accesses).  
- 📈 Complexity analysis:
  - Theoretical Big-O performance.  
  - Empirical results generated with Python (graphs of runtime vs input size).  
- 📝 Written report analyzing results and data structure trade-offs.  

---

## 🖥️ How to Compile

The project comes with a **Makefile** to build both implementations.

```bash
# Build linked list version (Stage 3)
make dict3

# Build Patricia tree version (Stage 4)
make dict4
```
This will generate executables dict3 and dict4.

---
🚀 How to Run
Both programs take 3 arguments:
1. Stage (3 for linked list, 4 for Patricia tree).
2. Input CSV dataset.
3. Output file name.

Queries are provided via standard input (.in files), and results are written to standard output.
Examples:
```bash
# Linked List (Stage 3)
./dict3 3 tests/dataset_1.csv output.out < tests/test1.in > output.stdout.out
./dict3 3 tests/dataset_15.csv output.out < tests/test15.in > output.stdout.out

# Patricia Tree (Stage 4)
./dict4 4 tests/dataset_1.csv output.out < tests/test1.in > output.stdout.out
./dict4 4 tests/dataset_1000.csv output.out < tests/test1000.in > output.stdout.out
./dict4 4 tests/dataset_full.csv output.out < tests/testfull.in > output.stdout.out
```
---
📊 Complexity Analysis
Linked List:
- Lookup = O(n) comparisons per query.
- Inefficient for large datasets.

Patricia Tree:
- Lookup = O(L) where L = length of the search key in bits.
- Much more efficient on large datasets.

---
📑 Report

The full analysis, including:
Methodology
- Tables of results
- Graphical comparisons
- Theoretical vs observed complexity
can be found in report.pdf

---
🛠️ Tech Stack
- Language: C (multi-file modular implementation)
- Python: For graphing and data visualization (matplotlib, pandas)
- Build: GNU Make
- Dataset: Simplified State Suburbs - Australia database (ABS)

