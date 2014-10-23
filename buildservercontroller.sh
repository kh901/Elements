g++ ServerController.cpp -c
g++ Account.cpp -c
g++ File.cpp -c
g++ Submission.cpp -c
g++ -o servercontroller ServerController.o Account.o Submission.o File.o -lsfml-network -lsfml-system
