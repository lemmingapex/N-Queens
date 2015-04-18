#include <iostream>
#include <stdlib.h>

using namespace std;

int N = 8;
int* queens;

int* colCount;
int* upperDiagCount;
int* lowerDiagCount;

// runtime: n
int BoardConflicts() {
	int conflicts=0;

	for(int i=0; i<2*N-1; i++) {
		if(i<N) {
			colCount[i]=0;
		}
		upperDiagCount[i]=lowerDiagCount[i]=0;
	}

	for(int i=0; i<N; i++) {
			if(queens[i] > -1) {
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

// runtime: n^2
void Initialize() {
	queens = new int[N];
	queens[0] = 0;

	colCount = new int[N];
	upperDiagCount = new int[(2*N)-1];
	lowerDiagCount = new int[(2*N)-1];

	// i=row index
	for(int i=1; i<N; i++) {
		queens[i] = -1;
	}
}

void Print() {
	for(int i=0; i<N; i++) {
		if(queens[i] < 0) {
			for(int j=0; j<N; j++) {
				cout << "| ";
			}
		} else {
			for(int j=0; j<queens[i]; j++) {
				cout << "| ";
			}
			cout << "|Q";
			for(int j=0; j<N-queens[i]-1; j++) {
				cout << "| ";
			}
		}
		cout << "|\n";
	}
	cout << "Conflicts: " << BoardConflicts() << "\n\n";
}

int Backtrack() {
	int steps = 0;
	int columnIndex = 0;
	int conflicts = BoardConflicts();
	while(queens[N-1] < 0 || conflicts > 0)	{
		while(conflicts != 0) {
			//Print();
			queens[columnIndex]++;
			while(queens[columnIndex] >= N) {
				queens[columnIndex] = -1;
				columnIndex--;
				queens[columnIndex]++;
			}
			steps++;
			conflicts = BoardConflicts();
		}
		columnIndex++;
		queens[columnIndex]=0;
		conflicts = BoardConflicts();
	}
	return steps;
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
	Initialize();
	int steps = Backtrack();

	if(N <= 20) {
		Print();
	}
	cout << "Number of steps to 0 conflicts: " << steps << "\n\n";

	return 0;
}
