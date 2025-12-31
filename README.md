# Hungarian Algorithm (C++ Implementation)

This repository contains a C++ implementation of the **Hungarian Algorithm** (also known as the Kuhn-Munkres algorithm). The Hungarian algorithm is a combinatorial optimization method that solves the assignment problem in polynomial time.

## Overview

The program finds the minimum cost assignment in an $N \times N$ cost matrix. It's particularly useful for scenarios where you need to match $N$ resources (e.g., workers) to $N$ tasks with the lowest total cost.

### Key Characteristics
- **Language**: C++
- **Dependencies**: Standard Library only (STL)
- **Memory Management**: Uses manual allocation (`new`/`delete[]`) for performance in algorithmic contexts.

## Getting Started

### Prerequisites

- A C++ compiler (e.g., `g++`, `clang++`, or MSVC).

### Compilation

Compile the program using your preferred compiler:

```bash
g++ -o hungarian hungarian_algorithm.cpp
```

## Usage

The program expects the following input format via standard input (`stdin`):
1.  **N**: The size of the $N \times N$ matrix.
2.  **Costs**: $N \times N$ integers representing the cost matrix.

### Example

**Input:**
```text
3
1 2 3
3 3 3
3 3 2
```

**Run command:**
```bash
./hungarian < input.txt
```

**Output:**
```text
1
2
3
```
*(The output indicates row 0 is assigned to column 1, row 1 to column 2, and row 2 to column 3 - using 1-based indexing for the result).*

## Implementation Details

The implementation follows the classic steps:
1.  **Row Reduction**: Subtracting the minimum value of each row from its elements.
2.  **Column Reduction**: Subtracting the minimum value of each column from its elements.
3.  **Optimal Assignment Search**: Efficiently covering zeros with the minimum number of lines.
4.  **Matrix Transformation**: If an assignment isn't found, the matrix is transformed using the minimum uncovered value to create more zeros.

## Testing

For verification, you can compare results with online tools like [Hungarian Algorithm Solver](https://www.hungarianalgorithm.com/solve.php).
