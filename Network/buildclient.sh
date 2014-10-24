g++ client.cpp -c
g++ ../Account.cpp -c
g++ ../File.cpp -c
g++ ../Submission.cpp -c
g++ -o clienttest client.o Submission.o Account.o File.o -lsfml-network -lsfml-system
