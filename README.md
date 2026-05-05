# Logistics Network & Cargo Management System

A high-performance Logistics Network and Cargo Management System developed in **C**, demonstrating advanced data structure applications including **Hash Tables** and **Graphs**. This project was built with a strict focus on **Dynamic Memory Allocation**, memory safety, and algorithmic efficiency.

## 🚀 Key Features

* **O(1) Package Lookup (Hash Table):** 
  * Utilizes the **Multiplication Method** (Type B) as the core hash function to evenly distribute package IDs.
  * Implements **Chaining (Linked Lists)** to seamlessly resolve Hash Collisions.
* **Logistics Routing (Graph & DFS):**
  * Models cities and route distances using an **Adjacency List** for maximum memory efficiency (avoids N x N matrix overhead).
  * Uses **Depth-First Search (DFS)** recursion to traverse the logistics network from the package's discovered location.
* **Memory Tracking & Safety:**
  * 100% dynamic memory allocation using `malloc` and `free`. 
  * Features an internal real-time memory tracking report that prints the Hexadecimal (`0x...`) RAM addresses of `HashNodes`, `EdgeNodes`, and `CityNodes`.
  * **0 Memory Leaks:** A strict teardown sequence cleanly returns all memory to the OS upon program completion.

## 📂 Project Structure

* `types.h`: Core data models (`Package`, `HashNode`, `HashTable`, `CityNode`, `EdgeNode`, `Graph`).
* `hash_table.c` / `.h`: Hash table implementation, multiplication method, chaining, and memory cleanup.
* `graph.c` / `.h`: Graph logic, bidirectional edge prepending (O(1)), and DFS traversal.
* `main.c`: Driver code containing the test scenario, dynamic data generation, and memory tracking output.
* `build.bat`: Windows compilation script setting up the MSYS2 MinGW GCC path.

## 🛠️ How to Build & Run

### Prerequisites
* **GCC Compiler** (MinGW/MSYS2 for Windows or standard `gcc` for Linux/macOS).

### Windows
1. Open Command Prompt or PowerShell in the project directory.
2. Run the build script:
   ```cmd
   .\build.bat
   ```
3. Run the executable:
   ```cmd
   .\lojistik_sistemi.exe
   ```

### Linux / macOS
You can manually compile the source files using GCC:
```bash
gcc main.c hash_table.c graph.c -o logistics_system
./logistics_system
```

## 🔍 Demonstration Scenario
When the program runs, it will:
1. Dynamically allocate memory for 5 cities (Istanbul, Ankara, Izmir, Bursa, Antalya) and map their distances.
2. Generate 5 packages and map them to the Hash Table.
3. Print a detailed **Memory Address Report** showing exactly where nodes are located in RAM.
4. Search for Package ID `104` in `O(1+a)` time.
5. Launch a **DFS Traversal** starting from Antalya (Package 104's location) to scan the entire connected logistics network.
6. Return all dynamically allocated memory to the OS.

---
*Developed by Berat Abdullah Mumcu for Data Structures.*
