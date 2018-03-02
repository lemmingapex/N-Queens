# N-Queens

Solves the [N-Queens problem](http://en.wikipedia.org/wiki/Eight_queens_puzzle) using various methods.

Methods:
* [Backtracking](http://en.wikipedia.org/wiki/Backtracking)
* [Genetic algorithm](http://en.wikipedia.org/wiki/Genetic_algorithm)
* Minimum-conflicts heuristic

## Compile It

make

## Run It

If no inputs are given, the following methods have reasonable defaults.

./Backtrack [Number of Queens]

./Genetic [Number of Queens] [Population Size] [Survival Rate] [Mutation Rate]

./MinimumConflict [Number of Queens]

## Thoughts

Each method has various limits to the input.  Backtracking is unusable with 30 or more queens.  The Genetic algorithm hits limits near 100 queens, but it is interesting to play with the algorithms parameters.  The minimum conflict method is by far the most robust and smartest algorithm.  Two big factors are key to its success.  First, a decent board initialization is required.  Initialization runs in O(n^2).  Second, a method that calcuates the number of conflicts on the board in linear time is also key for large boards.  The minimum conflict algorithm hits limits around 50000 queens.  I imagine that this method could solve a board as large as a million queens if provided a day of processing and decent hardware.

## Future Work
Other Methods? Threading potential?
