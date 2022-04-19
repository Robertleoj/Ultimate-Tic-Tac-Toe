# About
This is programming assignment 5 in the C++ programming course (FORC) at Reykjavík University, spring 2022.

The authors are   
* Róbert Leó Þormar Jónsson, `robertt20@ru.is`
* Indíana Líf Bergsteinsdóttir, `indiana18@ru.is`
* Ásdís Dagsdóttir, `asdisd20@ru.is`

# Introduction
This game is a twist on the classic Tic-Tac-Toe game, called Ultimate Tic-Tac-Toe (its rules are explained below). The game features a GUI, along with an AI the player can play against.

# The Rules
Its board is composed of nine smaller classic Tic-Tac-Toe boards, arranged to form a larger board. The players, cross and circle, take turns placing a cross or a circle in a square in one of the smaller boards. 

If a player manages to win three small boards that form a row or diagonal, that player wins the game. If there are no possible moves, and neither player won three small boards in a row, the winner is decided by the number of small boards they won. If they are equal in that regard, the game is a draw.

Now, when a player makes a move, they also decide where their opponent is allowed to play. If a player plays in the bottom right corner of a small board, the opponent must play in the bottom right board. A similar rule holds for the other squares on the smaller board. However, if it is not possible to play in the board the player was forced into, they can play in any available cell.

# The Game
The features in this game are humble in number, and the game is intended to be minimalist. There are two playing modes. A local two-player game can be played, where players take turns choosing their move with the mouse. 

The player can also choose to play agains an AI implemented with Monte Carlo Tree Search.

# Dependencies
To compile and run the game, you need 
* `g++` with the `c++17` standard.

* `SDL2` development package. Additionally, you will need the following `SDL2` libraries:  
    * `SDL2_image`
    * `SDL2_gfx`
    * `SDL2_ttf`     

    On ubuntu, these can be installed with
    ```bash
    sudo apt install libsdl2-2.0-0 libsdl2-dev libsdl2-gfx-1.0-0 libsdl2-gfx-dev libsdl2-image-2.0-0 libsdl2-image-dev libsdl2-ttf-2.0-0 libsdl2-ttf-dev
    ```

# Compiling and running
To compile the program, run
```
make
```
This produces a binary program named `ultimate_toe`. Thus, to run the game, run 
```
./ultimate_toe
```
in the terminal. You can run 
```
make clean
```
to delete all files produced during compilation.




