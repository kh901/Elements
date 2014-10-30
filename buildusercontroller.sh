g++ ./Source/Account.cpp -c
g++ ./Source/common.cpp -c
g++ ./Source/Conference.cpp -c
g++ ./Source/File.cpp -c
g++ ./Source/Menu.cpp -c
g++ ./Source/Phase.cpp -c
g++ ./Source/Submission.cpp -c
g++ ./Source/UserController.cpp -c
g++ ./Source/client_main.cpp -c
g++ ./Source/Review.cpp -c
g++ client_main.o Account.o Review.o common.o Conference.o File.o Menu.o Phase.o Submission.o UserController.o -lsfml-network -lsfml-system -o client -Wall
rm *.o
