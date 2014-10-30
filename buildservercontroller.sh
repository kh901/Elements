g++ ./Source/ServerController.cpp -c
g++ ./Source/Account.cpp -c
g++ ./Source/File.cpp -c
g++ ./Source/Conference.cpp -c
g++ ./Source/Submission.cpp -c
g++ ./Source/Phase.cpp -c
g++ ./Source/Database.cpp -c
g++ ./Source/common.cpp -c
g++ ./Source/Menu.cpp -c
g++ ./Source/server_main.cpp -c
g++ ./Source/Review.cpp -c
g++ ./Source/Database.cpp -c 
g++ -o servercontroller server_main.o ServerController.o Menu.o Account.o Submission.o Phase.o File.o Database.o common.o Conference.o Review.o -lsfml-network -lsfml-system -Wall
rm *.o
