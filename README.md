# Zappy
Serveur-client-graphical video game.


The game
--
Zappy is another classic of programming school. It is a game played online on a TCP server and by AI controlled players.

The idea is simple: the world of Trantor is vast and full of resources. The inhabitants of this world are Trantorians,
and they live pretty simply:

They roam the world to collect ressources, then when they have enough of ressources, they upgrade their level. This leveling
operation require a certain number of resources and a certain number of Trantorians reunited in the same place.

The team that gets 6 of their people to the max level is the winning team.

To increase their numbers, Trantorians can lay eggs to create slots in their teams.

The project
--
Zappy is a 5 man project on a 6 weeks duration. There are lots of rules and functionalities to implement and the project
is separated in three programs:

- ***The server***: it is the core of the project. It is where every game logic will be handled, and redirected toward the players.
It is implemented in C as a TCP/IP server.

- ***The client***: the client is a program that will connect to the server and "take decisions", when the client/player takes
a decision, he sends it to the server. But he cannot access its values by itself ; it is the server that decides everything.
The client may however asks the server what its values are. The client is implemented in C++.

- ***The graphical client*** (gfx): this program is the representation of our game. Like the player client,
it cannot influence the game, but it receives every needed message to display the game running on the server.
We used Unity to make that gfx.

Communication and game logic
--
The toughest part of the project was undeniably the server. We had to implement a defined protocol, and on top of it,
securize the server as much as possible to avoid cases like "unknown clients", "out of protocol messages", "wrong requests",
"packet data loss", and so on.

A big chunk of project time was spent on communication problems, and also optimization problems, as the server must be
able to receive datas from several players and process them as quickly as possible all the while avoiding
and handling packet losses.

The players' AI was also an interesting part, as we have players that communicate to find the required ressources, and
to reunite when the level up time comes.

---
In the end, we have a game that looks and run pretty well, in the limit of our defined boundaries. This project was an
excellent way to seriously apply our school's acquired TCP/IP knowledge.

The teamwork organization was also widely treated during this project, as with more people programming, the
code must be separated into sub modules, parts, or features, and the advancement of the project MUST be known at all times.
To achieve this, we worked under a simplified version of agile workflow, using Trello, schematics, and flexible
milestones.
