#include <iostream>
#include <limits>
#include <stdlib.h>
#include <vector>

using namespace std;

const int INF = numeric_limits<int>::max();

int N = 8;
int* queens;

int* colCount;
int* upperDiagCount;
int* lowerDiagCount;

// runtime: n
int BoardConflicts(int upperBound, int exrow) {
	int conflicts=0;

	for(int i=0; i<2*N-1; i++) {
		if(i<N) {
			colCount[i]=0;
		}
		upperDiagCount[i]=lowerDiagCount[i]=0;
	}

	for(int i=0; i<upperBound; i++) {
		if(i!=exrow) {
			colCount[queens[i]] += 1;
			upperDiagCount[queens[i]+i] += 1;
			lowerDiagCount[(N-queens[i])+i-1] += 1;
		}
	}
	for(int i=0; i<2*N-1; i++) {
		if(i<N) {
			conflicts += ((colCount[i]-1)*colCount[i])/2;
		}
		conflicts += ((upperDiagCount[i]-1)*upperDiagCount[i])/2;
		conflicts += ((lowerDiagCount[i]-1)*lowerDiagCount[i])/2;
	}
	return conflicts;
}

/*
// runtime: n
int QueenConflicts(int row) {
	int conflicts=0;
	int col=queens[row];
	for(int i=0; i<N; i++) {
		if(i!=row) {
			if(queens[i]==queens[row]) {
				conflicts++;
			}
			if(abs(row-i)==abs(col-queens[i])) {
				conflicts++;
			}
		}
	}
	return conflicts;
}
*/

// runtime: n^2
void Initialize() {
	queens = new int[N];

	colCount = new int[N];
	upperDiagCount = new int[(2*N)-1];
	lowerDiagCount = new int[(2*N)-1];

	vector<int> minConflictCols;
	int minConflicts=INF;
	int tempConflicts;

	// choose first queen randomly
	queens[0] = rand()%N;

	// i=row index
	for(int i=1; i<N; i++) {
		BoardConflicts(i-1, -1);
		minConflictCols.clear();
		minConflicts=INF;
		// j=col index
		for(int j=0; j<N; j++) {
			tempConflicts = ((colCount[j]+1)*colCount[j])/2;
			tempConflicts += ((upperDiagCount[j+i]+1)*upperDiagCount[j+i])/2;
			tempConflicts += ((lowerDiagCount[(N-j)+i-1]+1)*lowerDiagCount[(N-j)+i-1])/2;

			if(tempConflicts < minConflicts) {
				minConflicts=tempConflicts;
				minConflictCols.clear();
				minConflictCols.push_back(j);
			} else if(tempConflicts == minConflicts) {
				minConflictCols.push_back(j);
			}
		}
		queens[i] = minConflictCols[rand()%minConflictCols.size()];
	}
}

void Print() {
	for(int i=0; i<N; i++) {
		for(int j=0; j<queens[i]; j++) {
			cout << "| ";
		}
		cout << "|Q";
		for(int j=0; j<N-queens[i]-1; j++) {
			cout << "| ";
		}
		cout << "|\n";
	}
	cout << "Conflicts: " << BoardConflicts(N, -1) << "\n\n";
}

// calculates row with most conflicts
// runtime: n
int HighestConflicts() {
	int rowConflicts=0;
	int tempConflicts;
	vector<int> maxConflictRows;

	for(int i=0; i<N; i++) {
		tempConflicts = ((colCount[queens[i]]-1)*colCount[queens[i]])/2;
		tempConflicts += ((upperDiagCount[queens[i]+i]-1)*upperDiagCount[queens[i]+i])/2;
		tempConflicts += ((lowerDiagCount[(N-queens[i])+i-1]-1)*lowerDiagCount[(N-queens[i])+i-1])/2;

		if(tempConflicts > rowConflicts) {
			rowConflicts=tempConflicts;
			maxConflictRows.clear();
			maxConflictRows.push_back(i);
		} else if(tempConflicts == rowConflicts) {
			maxConflictRows.push_back(i);
		}
	}
	return maxConflictRows[rand()%maxConflictRows.size()];
}

// runtime: n
void MinConflicts() {
	int highestConflictRow = HighestConflicts();

	int minConflicts=INF;
	int tempConflicts;
	// min conflicts cols for queen
	vector<int> minConflictCols;

	//Print();
	BoardConflicts(N, highestConflictRow);

	// i=col index
	for(int i=0; i<N; i++) {
		tempConflicts = ((colCount[i]+1)*colCount[i])/2;
		tempConflicts += ((upperDiagCount[i+highestConflictRow]+1)*upperDiagCount[i+highestConflictRow])/2;
		tempConflicts += ((lowerDiagCount[(N-i)+highestConflictRow-1]+1)*lowerDiagCount[(N-i)+highestConflictRow-1])/2;

		if(tempConflicts < minConflicts) {
			minConflicts=tempConflicts;
			minConflictCols.clear();
			minConflictCols.push_back(i);
		} else if(tempConflicts == minConflicts) {
			minConflictCols.push_back(i);
		}
	}
	queens[highestConflictRow]=minConflictCols[rand()%minConflictCols.size()];
}

int main(int argc, const char *argv[]) {
	if(argc > 2) {
		cerr << "Usage:\n\t" << argv[0] << endl;
		cerr << "\t" << argv[0] << " [Number of Queens]" << endl;
		exit(1);
	} else if(argc == 2) {
		N = atoi(argv[1]);
	}
	if(N < 4) {
		cerr << "No solutions. The number of Queens is less than 4." << endl;
		exit(2);
	}

	cout << "Number of queens: " << N << endl;
	cout << "Initializing board..." << endl;

	srand(time(0));
	Initialize();

	if(N <= 20) {
		Print();
	}

	int previousConflicts = BoardConflicts(N, -1);
	int newConflicts;

	cout << "Initial conflicts: " << previousConflicts << endl << endl;

	int count = 0;
	int steps = 0;

	cout << "Solving..." << endl;

	while(previousConflicts != 0)	{
		MinConflicts();
		steps++;
		newConflicts = BoardConflicts(N, -1);
		if(previousConflicts == newConflicts) {
			count++;
			if(count>1) {
				queens[rand()%N] = rand()%N;
				count = 0;
			}
		}
		previousConflicts = newConflicts;
	}

	if(N <= 20) {
		Print();
	}
	cout << "Number of steps to 0 conflicts: " << steps << "\n\n";

	return 0;
}
