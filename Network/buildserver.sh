g++ server.cpp -c
g++ ../Account.cpp -c
g++ -o servertest server.o Account.o -lsfml-network -lsfml-system
