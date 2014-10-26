g++ ServerController.cpp -c
g++ Account.cpp -c
g++ File.cpp -c
g++ Conference.cpp -c
g++ Submission.cpp -c
g++ Phase.cpp -c
g++ Database.cpp -c
g++ common.cpp -c
g++ -o servercontroller ServerController.o Account.o Submission.o Phase.o File.o Database.o common.o Conference.o -lsfml-network -lsfml-system -Wall
