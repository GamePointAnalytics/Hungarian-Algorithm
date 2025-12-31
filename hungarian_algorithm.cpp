//This is a C++ implementation of the Hungarian Algorithm (aka Kuhn–Munkres Algorithm).
// March 2022


#include <iostream>
#include <vector>
#include <set>
#include <queue>
using namespace std;

// The C++ implementation of the Hungarian Matching Algorithm.
// Use this website to test https://www.hungarianalgorithm.com/solve.php
// A good python tutorial: https://python.plainenglish.io/hungarian-algorithm-introduction-python-implementation-93e7c0890e15

int* hungarian_algorithm(int** cost, int N) {
	int* row_min = new int[N];
	int* col_min = new int[N];
	for (int i = 0; i < N; i++) {
		row_min[i] = INT_MAX;
		col_min[i] = INT_MAX;
	}

	// Find the smallest value for each row
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (cost[i][j] < row_min[i]) {
				row_min[i] = cost[i][j];
			}
		}
	}

	// Subtract the smallest values from each column
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cost[i][j] -= row_min[i];
		}
	}

	// Find the smallest values per column 
	for (int j = 0; j < N; j++) {
		for (int i = 0; i < N; i++) {
			if (cost[i][j] < col_min[j]) {
				col_min[j] = cost[i][j];
			}
		}
	}

	// Subtract the smallest values from each column
	for (int j = 0; j < N; j++) {
		for (int i = 0; i < N; i++) {
			cost[i][j] -= col_min[j];
		}
	}

	set<pair<int, int>> assigned_zero_coord;

	bool* processed_row = new bool[N] { false };
	bool* marked_row = new bool[N] { true };
	bool* marked_col = new bool[N] { false };
	bool* assigned_row = new bool[N] { false };

	int* row_0_count = new int[N] { 0 };

	bool** assigned_0_tf = new bool* [N];
	for (int i = 0; i < N; i++) {
		assigned_0_tf[i] = new bool[N];
	}

	// Repeat until the number of marked col + unmarked row == N
	while (true) {

		assigned_zero_coord.clear();

		for (int i = 0; i < N; i++) {
			processed_row[i] = false;
			marked_row[i] = true;
			marked_col[i] = false;
			assigned_row[i] = false;
			row_0_count[i] = 0;
		}

		// Count the number of 0s per row
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (cost[i][j] == 0) {
					assigned_0_tf[i][j] = true;
					row_0_count[i]++;
				}
				else {
					assigned_0_tf[i][j] = false;
				}
			}
		}

		// Draw lines, may need to repeat this. 

		// Repeat until all rows are processed
		while (true) {
			// find the row with the smallest number of 0s
			int row_min_0_count = INT_MAX;
			int min_0_count = INT_MAX;
			for (int i = 0; i < N; i++) {
				if (!processed_row[i]) {
					if ((row_0_count[i] > 0) && (row_0_count[i] < min_0_count)) {
						min_0_count = row_0_count[i];
						row_min_0_count = i;
					}
					else if (row_0_count[i] == 0) {
						processed_row[i] = true;
					}
				}
			}

			if (min_0_count == INT_MAX) {
				// all the rows have been processed
				break;
			}

			// For this row, find the first 0 and save its (x, y) to a set assigned_zero_coord
			// In assigned[], mark this row as true. In marked[], set marked_row[row] as false;
			// If a row is not assigned, set assigned_row[row] as false. 
			// set marked_row[row] as true.
			// In a 2D array "assigned_0_tf", set this position as true. 
			// Mark all the other elements in the same row or column in "assigned_0_tf" 
			// as "false"?

			for (int j = 0; j < N; j++) {
				if ((cost[row_min_0_count][j] == 0) && (assigned_0_tf[row_min_0_count][j] == true)) {
					// Assign this 0 
					assigned_zero_coord.insert(make_pair(row_min_0_count, j));
					assigned_row[row_min_0_count] = true;
					marked_row[row_min_0_count] = false;

					// Set all the other elements in the row to be false
					// Adjust row 0 count
					for (int x = j + 1; x < N; x++) {
						if ((cost[row_min_0_count][x] == 0) &&
							(assigned_0_tf[row_min_0_count][x] == true)) {
							assigned_0_tf[row_min_0_count][x] = false;
							row_0_count[row_min_0_count]--;
						}
					}

					// Set all the other elements in the same column to be false
					// Adjust row 0 count
					for (int x = 0; x < row_min_0_count; x++) {
						if ((cost[x][j] == 0) &&
							(assigned_0_tf[x][j] == true)) {
							assigned_0_tf[x][j] = false;
							row_0_count[x]--;
						}
					}

					for (int x = row_min_0_count + 1; x < N; x++) {
						if ((cost[x][j] == 0) &&
							(assigned_0_tf[x][j] == true)) {
							assigned_0_tf[x][j] = false;
							row_0_count[x]--;
						}
					}
				}
			}

			processed_row[row_min_0_count] = true;
		}

		// Repeat until all the marked rows are processed
			// For each row where marked_row[row]==true, check every column to see if 
			// there is 0, if yes, set marked_col[col]=true. 
			// After each row, set processed_row[row] = true;

			// For each column in marked_col[col] = true, check every row. If there is
			// a true in any row of this column in assigned_0_tf, set marked_col[row]=true. 

		queue<int> queue_marked_rows;
		queue<int> queue_marked_cols;
		for (int i = 0; i < N; i++) {
			if (marked_row[i]) {
				queue_marked_rows.push(i);
			}
		}

		while (queue_marked_rows.size() > 0) {
			int cur_row = queue_marked_rows.front();
			queue_marked_rows.pop();

			for (int j = 0; j < N; j++) {
				if ((cost[cur_row][j] == 0) && (assigned_0_tf[cur_row][j] == false)) {
					queue_marked_cols.push(j);
					marked_col[j] = true;
				}
			}

			while (queue_marked_cols.size() > 0) {
				int cur_col = queue_marked_cols.front();
				queue_marked_cols.pop();

				for (int i = 0; i < N; i++) {
					if ((cost[i][cur_col] == 0) && (assigned_0_tf[i][cur_col] == true)) {
						queue_marked_rows.push(i);
						marked_row[i] = true;
					}
				}
			}
		}

		// If number of marked_col + unmarked_row = N, we are done. Break; 
		int num_unmarked_rows = 0;
		int num_marked_cols = 0;

		for (int i = 0; i < N; i++) {
			if (marked_row[i] == false) {
				num_unmarked_rows++;
			}
		}

		for (int j = 0; j < N; j++) {
			if (marked_col[j] == true) {
				num_marked_cols++;
			}
		}

		if ((num_unmarked_rows + num_marked_cols) == N) {
			// We are done
			break;
		}

		// Otherwise, find the smallest number in rows where marked_row[row] == true and 
		// marked_col[col] = false.
		int smallest_val = INT_MAX;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if ((marked_row[i] == true) && (marked_col[j] == false)) {
					// uncovered number
					if (cost[i][j] < smallest_val) {
						smallest_val = cost[i][j];
					}
				}
			}
		}

		// Subtract the min number from all the numbers where marked_row[row] == true and 
		// marked_col[col] = false.
		// Add the min numbers to the numbers where marked_row[row]==false and marked_col[col] == true.
		// Repeat
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if ((marked_row[i] == true) && (marked_col[j] == false)) {
					// uncovered number
					cost[i][j] -= smallest_val;
				}
				else if ((marked_row[i] == false) && (marked_col[j] == true)) {
					// covered number
					cost[i][j] += smallest_val;
				}
			}
		}
	}

	// The result is in assigned_zero_coord. 
	int* result = new int[N];

	set<pair<int, int>>::iterator it = assigned_zero_coord.begin();
	// Iterate till the end of set
	while (it != assigned_zero_coord.end()) {
		result[it->first] = it->second + 1;
		it++;
	}

	delete[] row_min;
	delete[] col_min;

	delete[] row_0_count;

	for (int i = 0; i < N; i++) {
		delete[] assigned_0_tf[i];
	}
	delete[] assigned_0_tf;

	delete[] processed_row;
	delete[] marked_row;
	delete[] marked_col;
	delete[] assigned_row;

	return result;
}

int main() {
	int N; // number of cows
	
	cin >> N;

	int** pref = new int* [N];
	int** cost = new int* [N];

	for (int i = 0; i < N; i++) {
		pref[i] = new int[N];
		cost[i] = new int[N];
	}

	// test

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> cost[i][j];
		}
	}

/*
	for (int i = 0; i < N; i++) {

		bool enter_max_cost = false; 
		// int cur_row_min = max_cost;

		for (int j = 0; j < N; j++) {
			cin >> pref[i][j];

			// start from 0, not 1
			pref[i][j]--;

			if (enter_max_cost) {
				cost[i][pref[i][j]] = max_cost;
			}
			else {
				if (pref[i][j] != i) {
					// the cost is the index of the preference list
					cost[i][pref[i][j]] = j;
				}
				else {
					cost[i][pref[i][j]] = j;
					enter_max_cost = true;
				}
			}

			// if (cost[i][j] < cur_row_min) {
			// 	  cur_row_min = cost[i][j];
			// }
		}
	}
*/

	int* result = hungarian_algorithm(cost, N);

	for (int i = 0; i < N; i++) {
		cout << result[i] << "\n";
	}

	delete[] result;

	for (int i = 0; i < N; i++) {
		delete[] cost[i];
		delete[] pref[i];
	}
	delete[] cost;
	delete[] pref;

	return 0;
}
