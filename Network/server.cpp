#include<iostream>
#include<SFML/Network.hpp>

using namespace std;

int main(){

	sf::TcpListener listener;
	
	if(listener.listen(60000)!=sf::Socket::Done){
	
		cout<<"error"<<endl;
		
	}
	
	sf::TcpSocket client;
	
	char data[100] = "hello world";
	if(listener.accept(client)== sf::Socket::Done){
	
		client.send(data,100);
			cout<<"Connected to Client"<<endl;
	
	}
	


}
