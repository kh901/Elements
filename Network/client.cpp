#include<iostream>
#include<SFML/Network.hpp>

using namespace std;
using namespace sf;	

int main(){

	Packet login;
	string username;
	string password;
	int level = 0;

	TcpSocket socket;
	Socket::Status status = socket.connect("localhost",60000);
	if(status == Socket::Done){
		char data[100] = "";
		size_t rec;
		socket.receive(data,100,rec);
		cout << data;
		cin >> username;
		cout << endl;
		char data[100] = "";
		socket.send(data,100,rec);
		socket.receive(data,100,rec);
		cout << data;
		cin >> password;
		cout << endl;
		data[100] = "";
		socket.send(data,100,rec);
		socket.receive(data,100,rec);
		cout << data;
		cin >> level;
		cout << endl;
		login << username << password << level;
		socket.send(login);
	}
	
	return 0
}
