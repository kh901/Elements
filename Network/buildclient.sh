g++ client.cpp -c
g++ ../Account.cpp -c
g++ ../File.cpp -c
g++ -o clienttest client.o Account.o File.o -lsfml-network -lsfml-system
