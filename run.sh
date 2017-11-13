# To start with move to the correct directory
cd conquest-engine-master/bin
# Then run with starter bots, once we finish making our bot presentable we can use our own via
# "gcc ../../main.cpp" since we're at the bin directory but our bot starts at that
# Want to make it so it takes in parameters through a file though, so something like gcc ../../main.cpp < infile
#Then in main.cpp we can carry over the argv into an init function which reads in parameter values from the file
java main.RunGame 0 0 0 "java bot.BotStarter" "java bot.BotStarter" 2>err.txt 1>../../out.txt
