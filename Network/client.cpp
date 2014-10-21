#include<iostream>
#include<SFML/Network.hpp>
#include<cstring>
#include<SFML/System/Time.hpp>

using namespace std;
using namespace sf;	

int main(){

	Packet login;

	string username;
	string password;
	int level = 0;

	TcpSocket socket;
	
	Time time = seconds(7);
	
	Socket::Status status = socket.connect("localhost",60000,time);
	
	if(status == Socket::Done){
		/*char data[100] = "";
		size_t rec;
		socket.receive(data,100,rec);*/
		bool valid = false;
		Packet response;
		while(valid==false){
		
			cout<<"Username: ";
			cin>>username;
			cout<<endl;
		
			cout<<"Password: ";
			cin>>password;
			cout<<endl;
		
			login << username << password;
			socket.send(login);
			socket.receive(response);
			response >> valid;
		}
	}
	
	return 0;
}
