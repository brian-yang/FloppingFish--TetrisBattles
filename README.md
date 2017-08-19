# TetrisBattles 2Player
## Project Members
* Anthony Liang
* Brian Yang
* Shaeq Ahmed

## Compilation Instructions
* Install the ncurses library:
```sudo apt-get install libncurses5-dev libncursesw5-dev```.
* Run ```make```.

## How to use
* After running ```make```, please do ```./server``` to start the server.
* Either on the current computer or another computer, connect to the server by doing ```./client <ip>``` where ip is the ip of the server.
* To start playing a game, a client and another client must both be connect to the server at the same time. For example, two clients must run ```./client <ip>```, with the ip being the same for the both of them to connect to the server and play.
* The game automatically resizes the window, but just in case, you could resize it manually to max size before the game begins.
* Play Tetris as you would normally do. Use the arrow keys to rotate and move the pieces left and right. Use the space bar to immediately put the piece down.
Use z to switch out pieces, and after a line has been completed, your score will be updated. Press q to quit.
* A player wins once the other player's pile of tetrimino blocks has piled up past the screen.
* See additional information/bugs to see a note on how many players can connect to the server at a time.

## List of all files
* client.c
* server.c
* networking.c
* networking.h
* common.h
* main.c
* tetris.c
* tetris.h
* DESIGN.txt
* DEVLOG.txt
* DOCUMENTATION.pdf
* README.md
* makefile

## Additional Information/Bugs
### Important notes
* Any computer can connect to the server and play as long as the client has the server's IP. However, the server can only accept a maximum of ```#NUM_USERS``` (which by default is set to 10) connections after it is first run before the server must be restarted. For example, you start the server, and let's say you disconnect and reconnect 10 times, even though you haven't played a game. After the 10th time, the server will refuse the connection, and the server must be restarted. 
* **However**, if you change the number of users (```#NUM_USERS```) in ```server.c```, then you can set the maximum number of connections to whatever number you want.
* Our documentation can be found in DOCUMENTATION.pdf

### Bugs
* The screen may flicker at times during the game depending how high your stacks are.
* The score board may appear weirdly if your score is more than one digit.

### Additional Information
* We tried send data through serialization, but we couldn't get it to work. We tried sending lines to opponents, but after trying several methods of sending data, we could not send lines to opponents. We had functions that could manually add lines to our own game, but not to our opponent's lines.
* We also tried implementing the bombs in Tetris Battles, but that didn't work out either. There may be a make error with the bomb functions, but the game should still work as it should.
* If you are interested in running with valgrind, do ```make vs``` to run the server with valgrind and ```make vc``` to run the client with valgrind. Note that the errors in valgrind result from ncurses' functions, and after looking it up, we found that these errors were normal with ncurses.
