#include <iostream>
#include <math.h>
#include <cstdlib>

using namespace std;

// board size NxN
int N = 8;

// inital population size
int PopSize;

// takes the top 'SurvivalRate'%
double SurvivalRate = 0.05;

// determines to percentage of random queen postions in a board to alter
double MutationRate;

class Board {
	public:
		int* queens;

		Board() {
			queens = new int[N];

			srand((unsigned)time(0));
			for(int j=0; j<N; j++) {
				queens[j]=(rand()%N);
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
			cout << "Conflicts: " << Conflicts() << "\n\n";
		}

		int Conflicts() {
			int rowConflict[N]={0};
			int upperDiagConflict[(2*N)-1]={0};
			int lowerDiagConflict[(2*N)-1]={0};

			for(int i=0; i<N; i++) {
				rowConflict[queens[i]] += 1;
				upperDiagConflict[queens[i]+i]+=1;
				lowerDiagConflict[(N-queens[i])+i-1]+=1;
			}
			int conflicts = 0;
			for(int i=0; i<2*N-1; i++) {
				if(i<N) {
					conflicts += ((rowConflict[i]-1)*rowConflict[i])/2;
				}
				conflicts += ((upperDiagConflict[i]-1)*upperDiagConflict[i])/2;
				conflicts += ((lowerDiagConflict[i]-1)*lowerDiagConflict[i])/2;
			}
			return conflicts;
		}

		void Mutate() {
			int mutationCount = max(min((int)floor(MutationRate*N), N), 1);
			//cout << mutationCount << endl;
			for(int i=0; i<mutationCount; i++) {
				//random pos
				int j=(rand()%N);
				//cout << "pos: " << j << endl;
				//random value
				char v =(rand()%N);
				queens[j]=v;
			}
		}

		Board Mate(Board Parent) {
			Board Child;
			// random pos between 1 and N-1
			int r=(rand()%(N-1))+1;
			for(int i=0; i<r; i++) {
				Child.queens[i]=queens[i];
			}
			for(int i=r; i<N; i++) {
				Child.queens[i]=Parent.queens[i];
			}
			return Child;
		}
};

// the population of boards
Board* Population;

void InitializePopulation() {
	Population = new Board[PopSize];
	for(int i=0; i<PopSize; i++) {
		Population[i] = Board();
	}
}

// modded quicksort
void sortPopulation(int L, int R) {
	int i = L, j = R;
	Board tmp;
	int pivot = Population[(L+R)/2].Conflicts();
	while(i <= j) {
		while(Population[i].Conflicts() < pivot) {
			i++;
		}

		while(Population[j].Conflicts() > pivot) {
			j--;
		}

		if(i <= j) {
			tmp = Population[i];
			Population[i] = Population[j];
			Population[j] = tmp;
			i++;
			j--;
		}
	};

	if(L < j) {
		sortPopulation(L, j);
	}
	if(i < R) {
		sortPopulation(i, R);
	}
}

int Genetic() {
	int numberOfMoves=0;

	int lastNumberOfConflictsInPopulation = -1;
	while(true) {
		// sort population by fitness level
		sortPopulation(0, PopSize-1);
		int fewestConflictsInPopulation = Population[0].Conflicts();
		if(lastNumberOfConflictsInPopulation != fewestConflictsInPopulation) {
			lastNumberOfConflictsInPopulation = fewestConflictsInPopulation;
			cout << "Conflicts: " << fewestConflictsInPopulation << endl;
		}
		if(fewestConflictsInPopulation == 0) {
			break;
		}

		// mate the top 'SurvivalRate' of the population
		// each board will mate with '1-SurvivalRate' others of the 'SurvivalRate' population
		// the childern will replace the other '1-SurvivalRate' of the population
		int cut = min(PopSize - 1, max((int)floor(PopSize*SurvivalRate), 1));
		int numberOfKids = floor(1/SurvivalRate)-1;
		for(int i=cut, j=0; i<PopSize; i+=numberOfKids, j++) {
			for(int k=0; k<numberOfKids; k++) {
				Population[i+k]=Population[j].Mate(Population[(j+k)%cut]);
			}
		}

		// mutate every board with the mutation rate
		for(int i=0; i<PopSize; i++) {
			Population[i].Mutate();
		}

		// now that we have a new generation incrment the number of moves
		numberOfMoves++;
	}

	return numberOfMoves;
}

int main(int argc, const char *argv[]) {
	if(argc == 5) {
		N = atoi(argv[1]);
		PopSize = atoi(argv[2]);
		SurvivalRate = atof(argv[3]);
		MutationRate = atof(argv[4]);
	} else if(argc > 2) {
		cerr << "Usage:\n\t" << argv[0] << endl;
		cerr << "\t" << argv[0] << " <Number of Queens>" << endl;
		cerr << "\t" << argv[0] << " <Number of Queens> <Population Size> <Survival Rate> <Mutation Rate>" << endl;
		exit(1);
	} else {
		if(argc == 2) {
			N = atoi(argv[1]);
		}
		PopSize = max(min(N*N, 10240), 512);
		MutationRate = 1.0/(double)N;
	}
	if(N < 4) {
		cerr << "No solutions. The number of Queens is less than 4." << endl;
		exit(2);
	}

	PopSize = max(min(PopSize, 10240), 512);
	SurvivalRate = max(min(SurvivalRate, 1.0), 0.0);
	MutationRate = max(min(MutationRate, 1.0), 0.0);

	cout << "Number of queens: " << N << endl;
	cout << "Population size: " << PopSize << endl;
	cout << "Survivial rate: " << SurvivalRate << endl;
	cout << "Mutation rate: " << MutationRate << endl;

	InitializePopulation();

	cout << "Number of Moves: " << Genetic() << endl;

	if(N<=20) {
		Population[0].Print();
	}

	return 0;
}
