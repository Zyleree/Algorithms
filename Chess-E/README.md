# Chess-E

*A component of the [Algorithms Collection](../README.md)*

Chess-E is my homegrown chess engine that I've been working on during weekends and late nights. It supports the UCI protocol so it'll work with pretty much any chess GUI out there.

## What's under

I've packed quite a few standard chess engine techniques into this project:

- Search stuff:
  * Alpha-beta pruning (obviously)
  * PVS for those sweet search optimizations
  * Quiescence search to handle those tricky tactical positions
  * LMR to skip the boring moves
  * Aspiration windows when we're getting close
  * Iterative deepening (because time management matters)

- Evaluation goodies:
  * Piece-square tables that actually make sense  
  * Pawn structure analysis (doubled, isolated, passed pawns)
  * King safety (because nobody likes their king in the center)
  * Mobility assessment (pieces need to breathe)

- Performance tricks:
  * Transposition tables (because we hate recalculating stuff)
  * Multi-threading (uses all your cores!)
  * Move ordering heuristics (MVV-LVA and all that jazz)

- Extra nice-to-haves:
  * Basic opening book
  * Simplified endgame tablebases
  * Post-game analysis tools

## Getting it running

You'll need:
- A C++17 compiler
- CMake 3.10+

Building is pretty straightforward:
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

Or just run the build script:
```bash
./build.bat build    # On Windows
make                # On Unix/Linux
```

## How to use it

### From the command line
```bash
./chess_engine    # Starts the UCI interface
./test_engine     # Runs the basic test suite
```

### With a chess GUI
1. Fire up your favorite chess GUI (Arena, Cutechess, Fritz, whatever)
2. Add a new engine
3. Point it to the chess_engine executable
4. Start playing!

## For developers

If you want to mess around with the code, check out the architecture doc in the `docs` folder. The main components are:

- Board representation (8x8 board with pieces)
- Move generation (legal moves only, thank you)
- Search (where the magic happens)  
- Evaluation (how good is this position anyway?)

To run the tests:
```bash
./test_engine           # Basic test suite
./test_evaluation       # Tests for the evaluation function
./test_search           # Tests for the search algorithms
```

## License & Contributing

This is under the MIT License - do whatever you want with it!

If you want to contribute, just fork it and send a PR. Check out [CONTRIBUTING.md](CONTRIBUTING.md) for some basic guidelines.

## Shout-outs

Huge thanks to:
- All those open source engines out there that showed me how this stuff works
- The guy who invented coffee

Happy chess hacking! 