# TeamIdiot
Welcome to ESPN8: The Ocho

This is a bot  
Please don't commit matthew  
Thanks  
Appreciate it  
We want a good grade  


K. So you want to run this with the engine? 
Also this is written assuming you use cygwin/bash
1. Get the engine into a file near the project
2. Make sure you have javac and java on your path
3. go to ../conquest-engine-location(not real name)/
4. Go to main/RunGame.java
5. Remove org.bson and com.mongodb imports
6. remove Db db
7. in go(0 remove path ending from both IORobot() declarations
8. Remove all things in saveGame() except maybe first couple lines
9. Go back to main directory for engine
10. Do these commands:   
mkdir bin
find . -type f \( -name "*.java" \) > sources.txt
javac -d bin -cp bin @sources.txt

Then to run go to bin and use the following command to run 2 starter bots
java main.RunGame 0 0 0 "java bot.BotStarter" "java bot.BotStarter" 2>err.txt 1>out.txt

If you want to use ours do the same but replace the quotes with our path and gcc

java main.RunGame 0 0 0 "java bot.BotStarter" "gcc /path/to/TeamIdiot/main.cpp" 2>err.txt 1>out.txt

Also if you want to output (to out.txt) the score include this in saveGame (for example)
System.out.println(this.engine.getRoundNr() - 1);
