# CuBirds

A C++ implementation of the card game **CuBirds**, designed to explore and evaluate optimal strategies through **brute-force Monte Carlo simulation** and **genetic algorithms**. This project simulates head-to-head matches between AI players, tunes parameters, and exports detailed statistics for analysis.

## Features

- Fully playable 2-player CuBirds logic
- Monte Carlo simulation-based player
- Genetic algorithm for evolving optimal strategy parameters
- Automatic statistical evaluation (win rates, fitness over generations)
- JSON-based results output for easy analysis
- Custom test modes (e.g. testing different starting setups)

## Installation

Clone the repository:
```bash
git clone https://github.com/NoelleBoer/CuBirds.git
cd CuBirds
```

Build the project using:
```bash
make
```

Run the executable:
```bash
./cubirds
```

## Strategy AI

### Genetic Algorithm

A tunable evolutionary approach to find optimal weights for decision-making. Each player is defined by six float parameters:

- `k`, `l`, `m`, `n`, `o`, `p`  
These control strategic behaviors such as bird selection, row preference, and collection timing.

Run the genetic algorithm by uncommenting the relevant line in `main()`:
```cpp
geneticAlgorithm(true); // Prints to terminal
geneticAlgorithm(false); // Saves to JSON
```

### Monte Carlo Simulation

A brute-force simulation player randomly explores all legal actions and selects the one with the best average outcome after simulating thousands of games.

Run Monte Carlo tests via:
```cpp
testMonteCarloVsGreedy();
testStartingSetups(true);  // Monte Carlo mode
testStartingSetups(false); // Standard mode
```

## Test Modes

### `testStartingSetups(MCGame, shell)`

Tests how starting with different birds in each player's collection influences win rates.

- `MCGame`: use Monte Carlo logic or not
- `shell`: print to console or output to `results/starting_setups_*.json`

### `geneticAlgorithm(shell)`

Evolves the best-performing player using crossover and mutation. Stores best-of-generation stats either in the shell or a JSON file.

### `printResultsJSONFile(...)`

Used for standardized output formatting for any experiment. Stores results in structured JSON under the `results/` folder.

## Output

All test and simulation results are saved to:

```bash
results/
├── starting_setups_standard.json
├── starting_setups_mc.json
├── genetic_algorithm.json
└── ...
```

These files contain win counts, player parameters, average turns per game, and more — making them easy to visualize or process in Python or Jupyter Notebooks.