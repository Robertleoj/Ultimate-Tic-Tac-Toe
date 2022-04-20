# TODOs for the project

## Multiple matches
Make the server handle multiple simultaneous matches

Simply put each match on a thread

## Online issues
* Handle the other player quitting
    * Tell the user that the connection has been lost
    * Or send a message that the other player quit
        * The server might send each player a status report before each move
        * Then it can tell the game that the player has left, or lost connection to the other player

* Handle the current player quitting
    * Stop the thread, and close the socket correctly
    * We might send the server something to tell it we quit

* Tell user the status when attempting connection
    * Connecting ... -> Waiting for opponent... -> start game
    * Connecting .. -> Connection failed

    * The user should be able to cancel at any time
        * The connect should be placed on a thread

