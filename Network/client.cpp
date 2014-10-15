#include<iostream>
#include<SFML/Network.hpp>

using namespace std;
using namespace sf;


int main(){

	TcpSocket socket;
	Socket::Status status = socket.connect("localhost",60000);
	if(status	== Socket::Done){
		cout<< "status" <<endl;
		char data[100] = "";
		size_t rec;
		socket.receive(data,100,rec);
		cout<<data<<endl;
	}




}


