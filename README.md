# 🛰️ Ship Navigation Simulator (3D Space Station Pathfinder)

## 🚀 Overview
3D search algorithm implementation for navigating a space station with features like elevators (E), start (S), and hangar (H) locations using depth-first search (DFS) or breadth-first search (BFS) based on CLI input. Input is parsed from either a map-style layout or a coordinate list.

ship.cpp → Core logic implementation
README.md → Project overview and usage guide
test-##- → test files 

## ⚙️ Features
- 3D map representation (levels × rows × columns)
- BFS (`--queue`) or DFS (`--stack`) search algorithms
- Elevator support between levels
- Two output formats:
  - `M`: Overlaid path on the map
  - `L`: Coordinate-based path trace
- Input via standard input (stdin)

## 🛠 Tech Stack
- C++
- STL: `vector`, `deque`, `stack`, `getopt`

## 🧠 Concepts Applied
- Graph traversal algorithms (BFS/DFS)
- 3D data structures
- Backtracking path reconstruction
- CLI argument parsing with `getopt_long

## 🧪 Usage Instructions

### 🔧 Compile
bash
g++ -std=c++11 -o ship ship.cpp

### Run 
./ship --queue --output M < input.txt
# or
./ship --stack --output L < input.txt

##Sample Input Format:

M 3 4
//////
S..E
.#.#
.H..
//////
....
.#.#
....
//////
....
.#E#
....

##Coordinate List Mode:
L 3 4
(0,0,0): S
(0,0,3): E
(0,2,1): #
...

Map List Mode: 
Start in level 0, row 0, column 0
//level 0
s..E
.#.#
.H..
...
...
