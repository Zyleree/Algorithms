# Inside Chess-E: How It All Works

Hey there! This doc breaks down how Chess-E is built under the hood. If you're curious about the engine's guts or want to contribute, this should help you get your bearings.

## The Big Picture

I built Chess-E with modularity in mind - everything is split into components that do specific jobs. Here's the cast of characters:

1. **Board Representation** - how we keep track of the chess position
2. **Move Generation** - figuring out what moves are legal
3. **Search Algorithm** - the brains that look ahead to find good moves
4. **Evaluation Function** - judging how good a position is
5. **UCI Interface** - talking to chess GUIs
6. **Utility Components** - all the other helpful bits and pieces

## The Components Up Close

### 1. Board Representation

For speed, we use bitboards (they're basically 64-bit integers where each bit represents a square):

- One bitboard for each piece type and color
- Bit operations make move generation and evaluation way faster
- We can make and unmake moves quickly during search

Look at `board.h/cpp` if you want to see how this works.

### 2. Move Generation

This is all about finding legal moves as quickly as possible:

- We pre-compute attack tables for pieces like bishops and rooks
- Each piece type has its own specialized generator
- Moves are encoded efficiently but still pack all the info we need

Check out `movegen.h/cpp` and `move.h` to see this in action.

### 3. Search Algorithm

This is where we explore different move sequences to find the best play:

- Alpha-beta pruning forms the core (it's like cutting branches off a tree before we waste time exploring them)
- Principal Variation Search helps us search even more efficiently
- Quiescence search handles tactical chaos so we don't miss combinations
- Iterative deepening lets us manage our thinking time
- Transposition table remembers positions we've already seen

The key files here are `search.h/cpp`, `alphabeta.h/cpp`, `principal_variation_search.h/cpp`, `quiescence_search.h/cpp`, `iterative_deepening.h/cpp`, and `transposition_table.h/cpp`.

### 4. Evaluation Function

When we can't search all the way to checkmate (which is most of the time), we need to judge positions:

- Material counting (queens are worth more than pawns, obviously!)
- Piece-square tables (knights are better in the center, rooks like open files)
- Pawn structure (doubled pawns are bad, passed pawns are good)
- King safety (is your king hidden behind a wall of pawns or out in the open?)
- Mobility (how many moves do your pieces have?)
- Pattern recognition (recognizing common good/bad positions)

All this lives in `evaluation.h/cpp`.

### 5. UCI Interface

This is how Chess-E talks to GUI programs:

- Parsing commands like "go" or "position"
- Handling the chess position
- Managing thinking time
- Dealing with options

See `uci.h/cpp` and `main.cpp` for the details.

### 6. Utility Components

The supporting cast that makes everything work better:

- `endgame_tablebase.h/cpp`: Perfect play for positions with few pieces
- `opening_book.h/cpp`: Pre-computed good opening moves
- `time_management.h/cpp`: Making sure we don't flag in time controls
- `thread_pool.h/cpp`: Spreading the search across multiple CPU cores

## How It All Flows Together

1. GUI sends a command through UCI
2. We parse and process the command
3. For "go" commands, we fire up the search
4. The search uses move generation to build a tree of possibilities
5. We evaluate positions using our evaluation function
6. We return the best move to the UCI interface
7. UCI tells the GUI what move we picked

## Search Tricks Up Our Sleeve

Chess engines need to be clever about search to be fast. Here are some tricks we use:

- **Move Ordering**: Looking at captures and good moves first makes alpha-beta work better
- **Null Move Pruning**: "What if I just skipped my turn?" - helps cut off bad lines
- **Late Move Reduction**: "These moves look unpromising, let's not search them as deeply"
- **Aspiration Windows**: Assuming the evaluation won't change too drastically between iterations
- **Killer Move Heuristic**: "This move worked great in a similar position, let's try it early"
- **History Heuristic**: Keeping track of which moves tend to be good across the entire search

## Running on Multiple Cores

Chess-E can use multiple CPU threads:

- We have a thread pool to manage our worker threads
- A shared transposition table (with proper synchronization to avoid race conditions)
- We use Lazy SMP (Symmetric Multi-Processing) which is simple but effective

## What's Next on the Roadmap

Things I'd like to add or improve:

- Neural network evaluation (the cool new thing all engines are doing)
- NNUE implementation (Efficiently Updatable Neural Network - faster than traditional nets)
- Smarter search pruning 
- Better parallel search to use those multi-core CPUs 