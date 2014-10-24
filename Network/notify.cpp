#include <iostream>
#include <string>
#include <map>
#include <queue>

typedef std::map< std::string, std::queue<std::string> > NotifyMap;

void displayNotifications(NotifyMap &map, const std::string &user);
void addNotification(NotifyMap &map, const std::string &user, const std::string &str);
void getNotifications(NotifyMap &map, const std::string &user, std::vector<std::string> &list);

void displayNotifications(NotifyMap &map, const std::string &user)
{
	NotifyMap::iterator it;
	it = map.find(user);
	std::cout << "Checking notifications for " << user << std::endl;
	if (it != map.end() && !map[user].empty())
	{
		// show all queued notifications
		while (!map[user].empty())
		{
			// get the front most notification
			std::string get = map[user].front();
			map[user].pop();
			std::cout << get << std::endl;
		} 
	}
	else
	{
		std::cout << "No notifications!" << std::endl;
	}
}

void getNotifications(NotifyMap &map, const std::string &user, std::vector<std::string> &list)
{
	int size = map[user].size();
	list.reserve(size);
	if (!map[user].empty())
	{
		while (!map[user].empty())
		{
			list.push_back(map[user].front());
			map[user].pop();
		} 
	}
}

void addNotification(NotifyMap &map, const std::string &user, const std::string &str)
{
	map[user].push(str);
}

typedef std::map<std::string, int> AllocMap;

bool incrementAlloc(AllocMap &map, const std::string &id, const int max)
{
	AllocMap::iterator it;
	it = map.find(id);
	std::cout << "Incrementing for confernce: " << id << std::endl;
	if (it == map.end())
	{
		std::cout << "Creating new row" << std::endl;
		map[id] = 0;
	}
	if (map[id] < max)
	{
		map[id] += 1;
		std::cout << "Incrementing counter " << map[id] << " / " << max << std::endl;
		return true;
	}
	std::cout << "Counter at max " << map[id] << " / " << max << std::endl;
	return false;
}

int main ()
{
	NotifyMap map;
	std::string key = "Alice";
	addNotification(map, key, "Your car is in a 5 minute parking zone.");
	addNotification(map, key, "Your car has been parked for almost 5 minutes.");
	addNotification(map, key, "Your car has been towed.");
	addNotification(map, key, "Your car has been crushed into a tiny cube.");
	addNotification(map, key, "Your cube has 5 minutes to vacate the parking zone.");
	displayNotifications(map, key);
	displayNotifications(map, "Bob");
	addNotification(map, "Bob", "Hi bob, welcome!");
	displayNotifications(map, "Bob");
	displayNotifications(map, "Potato");
	addNotification(map, "Test", "Hi");
	displayNotifications(map, "Test");
	
	AllocMap allocMap;
	incrementAlloc(allocMap, "a", 3);
	incrementAlloc(allocMap, "a", 3);
	incrementAlloc(allocMap, "a", 3);
	incrementAlloc(allocMap, "b", 1);
	incrementAlloc(allocMap, "a", 3);
}
