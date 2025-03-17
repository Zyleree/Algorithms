# Getting Started with Chess-E

Hey there, chess enthusiast! This guide will walk you through setting up and using Chess-E, my homegrown chess engine. Whether you're a casual player or serious analyst, I've tried to make things simple and straightforward.

## What's in this guide
1. [Setting up Chess-E](#installation)
2. [Using the command line](#command-line-usage)
3. [Hooking it up to chess GUIs](#using-with-chess-guis)
4. [Tweaking engine settings](#engine-options)
5. [Help! Something's not working](#troubleshooting)

## Installation

### Building it yourself
```bash
# Grab the code
git clone [repository-url]
cd Chess-E

# Set up a build folder
mkdir build && cd build

# Build it!
cmake ..
cmake --build .
```

### Ready-made binaries
If you'd rather skip the build process, look for pre-built versions on the releases page - though I can't always guarantee they'll be up to date

## Command Line Usage

You can run Chess-E straight from your terminal:

```bash
./chess_engine
```

This fires up the engine in UCI mode. Here are some commands you can try:

```
uci                         # Say hello to the engine
isready                     # Check if it's ready to roll
position startpos           # Set up the starting position
position startpos moves e2e4 e7e5   # After 1.e4 e5
go depth 8                  # Think 8 moves ahead
go movetime 5000            # Think for 5 seconds
stop                        # Stop thinking
quit                        # Shut down
```

## Using with Chess GUIs

Chess is more fun with a proper board! Here's how to connect Chess-E to some popular interfaces:

### Arena Chess GUI

1. Grab Arena from [their website](http://www.playwitharena.de)
2. Go to `Engines` → `Install New Engine`
3. Find your Chess-E executable
4. Tweak the settings if you want
5. Start playing!

### Cutechess

1. Download Cutechess from [their site](https://cutechess.com)
2. Go to `Tools` → `Settings` → `Engines` → `Add`
3. Fill in:
   - Name: Chess-E (or whatever you want to call it)
   - Command: Path to your chess_engine executable
   - Protocol: UCI
4. Configure any options
5. Start a game or set up a tournament

### Fritz / Chessbase

1. In Fritz/Chessbase, hit `Engine` → `Create UCI Engine`
2. Find your Chess-E executable
3. Configure as you like
4. Start analyzing or playing

### Lichess Desktop

1. In Lichess Desktop, go to `Tools` → `Analysis Engine` → `Manage`
2. Click `Add Engine`
3. Find your Chess-E executable
4. Set it up how you want
5. Analyze away!

## Engine Options

Here are the knobs and dials you can tweak to get the most out of Chess-E:

| Option | What it does | Default | Range |
|--------|-------------|---------------|-------|
| Hash | Memory for the "brain" (MB) | 64 | 1-1024 |
| Threads | How many CPU cores to use | 1 | 1-64 |
| MultiPV | Number of best moves to show | 1 | 1-10 |
| Contempt | How much the engine avoids draws | 0 | -100 to 100 |
| Skill Level | How strong to play | 20 | 0-20 |
| Use Opening Book | Play book moves at the start? | true | true/false |
| Use Tablebase | Use perfect endgame databases? | true | true/false |

To change a setting in UCI mode:
```
setoption name Hash value 128
```

## Advanced Analysis

Want to dig deeper? Chess-E has some cool analysis features:

### Multiple lines of analysis
```
go depth 20 multipv 3
```
Shows the top 3 moves to depth 20.

### Never-ending analysis
```
go infinite
```
Thinks until you tell it to stop with `stop`.

### Timed analysis
```
go movetime 30000
```
Thinks for 30 seconds.

### Node-limited analysis
```
go nodes 1000000
```
Searches a million positions then stops.

## Troubleshooting

Sometimes things go sideways. Here's how to get back on track:

### Engine crashes
- Update to the latest version
- If you've set Hash too high, dial it back
- Try using fewer Threads
- Make sure your computer isn't overheating

### Engine feels sluggish
- Bump up the Hash if you've got RAM to spare
- Use more Threads if you have a multi-core CPU
- Close other heavy applications

### GUI can't find the engine
- Check that the executable has the right permissions
- Try using the full path to the executable
- Some GUIs are picky about where engines live - check their docs

### Opening book problems
- Make sure the book file exists and is in the right spot
- The book might be corrupted - try redownloading it
- Check that book usage is turned on in the options

## Need more help?

If you're still stuck:
1. Take a peek at open GitHub issues - someone might have had the same problem
2. If it's something new, please open an issue
3. Drop by our community forums - we're a friendly bunch! 