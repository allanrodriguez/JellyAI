This is my part of a team senior design project for the computer engineering major at the University of California, Irvine. The complete design, known as **Jelly**, is a system that allows people to play against a chess engine in real life, with a robot arm assisted by an optical piece detection algorithm making the engine's moves on a physical chessboard. Because this system doesn't work completely with all parts present, I've only provided the chess engine, which I designed, along with a small command-line interface to enter moves manually.

**This compiles with the C++11 standard, so GCC 4.8.1 or above is required.**

To compile, run `make JellyAI` in a terminal.

To run, navigate to the `bin` folder and run `JellyAI` in a terminal.

To clean, run `make clean` in a terminal.

Places are determined by their file letter, followed by their rank number (e.g. the bottom-left square is **a1**).

Enjoy!