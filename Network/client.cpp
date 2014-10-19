#include<iostream>
#include<SFML/Network.hpp>

using namespace std;
using namespace sf;


int main(){

	int option[1];
	TcpSocket socket;
	Socket::Status status = socket.connect("localhost",60000);
	if(status == Socket::Done){
		//cout<< "status" <<endl;
		char data[100] = "";
		size_t rec;
		socket.receive(data,100,rec);
		cout<<data<<endl;
		cin >> option[0];
		if (option[0] > 5 || option[0] < 1)
		{
			cout << "ERROR" << endl;
			return -1;
		}
		socket.send(option, 1);
	}
	
	return 0;
}


