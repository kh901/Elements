g++ server.cpp -c
g++ ../Account.cpp -c
g++ ../File.cpp -c
g++ ../Submission.cpp -c
g++ -o servertest server.o Account.o Submission.o File.o -lsfml-network -lsfml-system
