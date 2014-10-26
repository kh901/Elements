g++ Account.cpp -c
g++ common.cpp -c
g++ Conference.cpp -c
g++ File.cpp -c
g++ Menu.cpp -c
g++ Phase.cpp -c
g++ Submission.cpp -c
g++ UserController.cpp -c
g++ Account.o common.o Conference.o File.o Menu.o Phase.o Submission.o UserController.o -lsfml-network -lsfml-system -o usercontroller -Wall
