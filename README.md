### Systems Level Programming Final Project --- MMORPG ###
#### Kevin Hwang, Ricky Wu, Ruochong Wu, Matthew Zhu ####

** CURRENT STATUS ** - Gameplay code written but bug prevents it from working. Chatroom complete and functional.

** Files/directories in directory neccesary for compilation**
- accounts/
- client.c
- combat.c (currently none functional but coded)
- dungeons/
- makefile
- networking.c
- networking.h
- server.c

** Directions **
1. run make to compile
2. run ./server
3. run ./client <IP ADDRESS>. No IP address given defaults to loopback address
4. run as many ./client instances as desired
5. login or register account for each client instance
6. Once logged in, chatroom opened by default, type message and press enter to sends

** Full directions once bug is fixed **
7. Select class
8. Once all players have connected and selected class, use command /startbattle to initiate battle
9. Battle begins, turned based combat, each party member gets to attack, use magic, or use item during their turn (button instructions displayed on screen)
10. If win battle move on to next battle, if lose, return to lobby
