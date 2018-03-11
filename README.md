# Trigger snake

<center><img src="https://ahstat.github.io/images/2014-10-11-Trigger-snake/snake_intro.png" alt="Introduction of the game"/></center>

**Trigger snake** is a snake game with new features, making it fast, lively and challenging. The related blog post describes the main features and can be found here: https://ahstat.github.io/Trigger-snake/ You can directly play the game <a href="https://github.com/ahstat/trigger-snake/raw/master/release/trigger-snake_win8.zip">by downloading it here (for Windows)</a>.

<center><img src="https://ahstat.github.io/images/2014-10-11-Trigger-snake/ingame.png" alt="Game screenshot"/></center>

**Components.**
The game has been written in C++ with Qt 4. Five cpp files are present:
- main.cpp loads the widget. This widget includes a Board and two LCDRange,
- LCDRange.cpp contains code for LCD graphical elements. We use it for current score (1557 in the previous screenshot) and hi-score (16140),
- Board.cpp manages the board and mechanisms of the game (snake and apples behaviors),
- Helpers.cpp includes helpers for Board.cpp,
- Alea.cpp allows sampling random variables.

**Inputs.**
When the game has been lauched, you can type any key to begin a new game. Use directional keys to move the snake in the four directions. Other special keys are:
- R -- restart a game in-game or after losing,
- D -- display density of snake's positions,
- E -- display density of snake's directions,
- W -- display density of snake's non-oriented directions,
- A -- display density of apples' positions,
- 1 -- cheat code: change apple's position,
- 2 -- cheat code: increase snake's length by one,
- 3 -- cheat code: win current game.

You can get details about density boards in <a href="https://ahstat.github.io/Trigger-snake/">my blog post</a>.

**Outputs.**
Two files are saved in current directory:
- hiscore.txt -- contains crypted value of the hi-score,
- output.txt -- contains whole history of all played games.
