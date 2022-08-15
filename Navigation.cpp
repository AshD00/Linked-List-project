#include "Navigation.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "ACW_Wrapper.h"

using namespace std;

Navigation::Navigation() : _outFile("Output.txt")
{
}

Navigation::~Navigation()
{
}

bool Navigation::ProcessCommand(const string& commandString)
{
	istringstream inString(commandString);
	string command;
	inString >> command;

	// Add your code here

	if (command == "MaxDist") 
	{
		MaxDist(command);
		return true;
	}
	if (command == "MaxLink") 
	{
		MaxLink(command);
		return true;
	}
	if (command == "FindDist") 
	{
		string first, second;
		inString >> first >> second;
		FindDist(command, first, second, true);
		return true;
	}
	if (command == "FindNeighbour") 
	{
		string first;
		inString >> first;
		const int node = stoi(first);
		FindNeighbour(command, node);
		return true;
	}
	if (command == "Check") 
	{
		string first;
		vector<int> nodeVec;
		string mode;
		int i = 1;
		while (!inString.eof())
		{
			std::getline(inString, first, ' ');

			if (i == 1) {
			}
			else if (i == 2) {
				mode = first;
			}
			else {
				const int node = stoi(first);
				nodeVec.push_back(node);
			}
			i++;
		}
		Check(command, mode, nodeVec);
		return true;
	}
	if (command == "FindRoute")
	{
		string first, second, third;
		inString >> first >> second >> third;
		const int node1 = stoi(second), node2 = stoi(third);
		_outFile << command << " " << first << " " << node1 << " " << node2 << endl;
		FindRoute(first, node1, node2);
		return true;
	}
	if (command == "FindShortestRoute")
	{
		string first, second, third;
		inString >> first >> second >> third;
		const int node1 = stoi(second), node2 = stoi(third);
		_outFile << command << " " << first << " " << node1 << " " << node2 << endl;
		ShortestRoute(first, node1, node2);
		return true;
	}
	return false;
}

bool Navigation::BuildNetwork(const string &fileNamePlaces, const string &fileNameLinks)
{
	fstream finPlaces(fileNamePlaces);
	fstream finLinks(fileNameLinks);
	if (finPlaces.fail() || finLinks.fail()) return false;

	// ****** Add your code here *******

	struct Place
	{
		std::string name;
		int id = 0, arrPos = 0;
		double x = 0, y = 0;
	};

	int count = 0;
	while (!finPlaces.eof()) 
	{
		string name, id, latitude, longitude;
		
		getline(finPlaces, name, ',');
		getline(finPlaces, id, ',');
		getline(finPlaces, latitude, ',');
		getline(finPlaces, longitude);
		double x, y;
		LLtoUTM(stod(latitude), stod(longitude), x, y);

		places.push_back(new Node(name, stoi(id), x, y));		
	}
	
	//Creating an adjacency graph for the links
	count = 0;
	while (!finLinks.eof())
	{
		string start, end, vehicle;
		double distance = 0;
		vector <string> modes;
	
		getline(finLinks, start, ',');
		getline(finLinks, end, ',');
		getline(finLinks, vehicle);
		const int startID = stoi(start), endID = stoi(end);
		
		if (vehicle == "Ship") {
			modes.push_back("Ship");
			modes.push_back("Bus");
			modes.push_back("Car");
			modes.push_back("Bike");
			modes.push_back("Foot");
		}
		else if (vehicle == "Rail") {
			modes.push_back("Rail");
			modes.push_back("Bike");
			modes.push_back("Foot");
		}
		else if (vehicle == "Bus") {
			modes.push_back("Bus");
			modes.push_back("Car");
			modes.push_back("Bike");
			modes.push_back("Bike");
		}
		else if (vehicle == "Car") {
			modes.push_back("Car");
			modes.push_back("Bike");
			modes.push_back("Foot");
		}
		else if (vehicle == "Bike") {
			modes.push_back("Bike");
			modes.push_back("Foot");
		}		
		else if (vehicle == "Foot") {
			modes.push_back("Foot");
		}

		for (int i = 0; i < places.size(); i++) 
		{
			if (places[i]->getID() == startID) 
			{
				for (int n = 0; n < places.size(); n++)
				{
					if (places[n]->getID() == endID)
					{
						distance = pow(pow((places[i]->getX() - places[n]->getX()), 2) + pow((places[i]->getY() - places[n]->getY()), 2), 0.5);
						places[i]->createArc(new Arc(places[n], distance, modes));
						places[n]->createArc(new Arc(places[i], distance, modes));
						break;
					}
				}
				break;
			}
		}
	}

	// example code of using the LLtoUTM function from ACW_Wrapper to convert 
	// latitude and longitude values into x and y global coordinates:
	//double latitude = 53.770, longitude = -0.368, x, y;
	//LLtoUTM(latitude, longitude, x, y);
	return true;
}

// Add your code here

void Navigation::MaxDist(const string& commandString)
{
	double distMax = 0, distance = 0;
	string startName, endName;

	for (int i = 0; i < places.size(); i++) 
	{
		for (int n = 0; n < places.size(); n++)
		{
			distance = pow(pow((places[i]->getX() - places[n]->getX()), 2) + pow((places[i]->getY() - places[n]->getY()), 2), 0.5);
			if (distance > distMax) 
			{
				distMax = distance;
				startName = places[i]->getName();
				endName = places[n]->getName();
			}
		}
	}

	_outFile << commandString << endl << startName << ", " << endName << ", " << distMax << endl << endl;
}

void Navigation::MaxLink(const string& commandString)
{
	double distMax = 0;
	int startPoint = 0, endPoint = 0;

	for (int i = 0; i < places.size(); i++) 
	{
		for (int n = 0; n < places[i]->arcSize(); n++) 
		{
			if (places[i]->getArc(n)->getDist() > distMax) 
			{
				startPoint = places[i]->getID();
				endPoint = places[i]->getArc(n)->getEnd()->getID();
				distMax = places[i]->getArc(n)->getDist();
			}
		}

	}

	_outFile << commandString << endl << startPoint << ", " << endPoint << ", " << distMax << endl << endl;
}

double Navigation::FindDist(const string& commandString, const string &startName, const string &endName, const bool out)
{
	double distance = 0;
	const int startPoint = stoi(startName), endPoint = stoi(endName);
	string sName = startName, eName = endName;

	if (out) {
		sName = commandString.substr(0, commandString.find(' '));
	}

	for (int i = 0; i < places.size(); i++) 
	{
		if (places[i]->getID() == startPoint)
		{
			for (int n = 0; n < places.size(); n++)
			{
				if (places[n]->getID() == endPoint)
				{
					distance = pow(pow((places[i]->getX() - places[n]->getX()), 2) + pow((places[i]->getY() - places[n]->getY()), 2), 0.5);
					if (out) 
					{
						sName = places[i]->getName();
						eName = places[n]->getName();
					}
				}
			}
		}
	}
	if (out) 
	{
		_outFile << commandString << ' ' << startPoint << ' ' << endPoint << endl << startName << ", " << endName << ", " << distance << endl << endl;
	}
	return distance;
}

void Navigation::FindNeighbour(const string& commandString, const int node) 
{
	_outFile << commandString << ' ' << node << endl;

	for (int i = 0; i < places.size(); i++) 
	{
		if (places[i]->getID() == node)
		{
			for (int n = 0; n < places[i]->arcSize(); n++) 
			{
				_outFile << places[i]->getArc(n)->getEnd()->getID() << endl;
			}
		}
	}
	_outFile << endl;
}

void Navigation::Check(const string& commandString, const string &mode, const vector<int> &nodes)
{
	bool Passed = true;
	_outFile << commandString << " " << mode << " ";
	for (int i = 0; i < nodes.size(); i++) {
		_outFile << nodes[i] << " ";
	}
	_outFile << endl;
	
	//string modeCheck;

	for (int i = 0; i < places.size(); i++)
	{
		for (int n = 0; n < nodes.size()-1; n++) 
		{
			if (places[i]->getID() == nodes[n])
			{
				for (int x = 0; x < places[i]->arcSize(); x++) 
				{
					if (places[i]->getArc(x)->getEnd()->getID() == nodes[static_cast<__int64>(n) + 1]) {
						for (int y = 0; y < places[i]->getArc(x)->modesSize(); y++)
						{
							if (places[i]->getArc(x)->getMode(y) == mode)
							{
								_outFile << nodes[n] << ", " << nodes[static_cast<__int64>(n) + 1] << ", " << "PASS" << endl;
								break;
							}
							else if (y == places[i]->getArc(x)->modesSize() -1)
							{
								_outFile << nodes[n] << ", " << nodes[static_cast<__int64>(n) + 1] << ", " << "FAIL" << endl;
								Passed = false;
								break;
							}
							if (!Passed)
							{
								break;
							}
						}
					}
					if (!Passed)
					{
						break;
					}
				}
			}
			if (!Passed)
			{
				break;
			}
		}
		if (!Passed)
		{
			break;
		}
	}
	_outFile << endl;
}

void Navigation::FindRoute(const string& mode, const int startNode, const int endNode) 
{
	vector<Node*> open;
	vector<Node*> closed;
	vector<Node*> path;
	const size_t size = places.size();
	Node* start = new Node;
	Node* end = new Node;
	bool found = false;

	for (int i = 0; i < size; i++)
	{
		places[i]->setDist(FindDist("", to_string(places[i]->getID()), to_string(endNode), false));
		places[i]->setFound(false);
		if (places[i]->getID() == startNode)
		{
			start = places[i];
			start->setFound(true);
			start->setF(0);
			open.push_back(start);
		}
		else if (places[i]->getID() == endNode)
		{
			end = places[i];
		}
	}

	while (open.size() > 0 && !found)
	{
		int minDist = INT_MAX;
		
		Node* currentNode = new Node();

		for (int i = 0; i < open.size(); i++)
		{
			if (open[i]->getDist() < minDist)
			{
				currentNode = open[i];
				minDist = currentNode->getDist();
			}
		}
		closed.push_back(currentNode);
		path.push_back(currentNode);
		for (int i = 0; i < open.size(); i++)
		{
			if (open[i] == currentNode) 
			{
				open.erase(open.begin() + i);
			}
		}

		if (currentNode == end) 
		{
			found = true;
		}
		else 
		{	
			for (int x = 0; x < currentNode->arcSize(); x++) 
			{
				bool viable = true;
				for (int y = 0; y < closed.size(); y++)
				{
					if (currentNode->getArc(x)->getEnd() == closed[y])
					{
						viable = false;
						break;
					}
				}
				if (viable) 
				{
					for (int o = 0; o < currentNode->getArc(x)->modesSize(); o++)
					{
						viable = false;
						if (currentNode->getArc(x)->getMode(o) == mode)
						{
							viable = true;
							break;
						}
					}
					if (viable)
					{
						currentNode->getArc(x)->getEnd()->setG(currentNode->getArc(x)->getEnd()->getG() + currentNode->getG());
						currentNode->getArc(x)->getEnd()->setF(currentNode->getArc(x)->getEnd()->getG() + currentNode->getArc(x)->getEnd()->getDist());
						viable = true;
						for (int z = 0; z < open.size(); z++)
						{
							if (currentNode->getArc(x)->getEnd() == open[z] || currentNode->getArc(x)->getEnd()->getG() > open[z]->getG())
							{
								viable = false;
								break;
							}
						}
						if (viable)
						{
							open.push_back(currentNode->getArc(x)->getEnd());
						}
					}
				}
			}
		}
	}

	if (found) 
	{
		for (int i = 0; i < path.size(); i++) 
		{
			_outFile << path[i]->getID() << endl;
		}	
	}
	else 
	{
		_outFile << "Fail" << endl;
	}
	_outFile << endl;
}

void Navigation::ShortestRoute(const string &mode, const int startNode, const int endNode)
{
	vector<Node*> Q;
	Node* v;
	const size_t size = places.size();
	vector <Node*> routes;
	bool found = false;
	Node* start = new Node;
	Node* end = new Node;
	Node* nextNode = new Node;

	for (int i = 0; i < size; i++) 
	{
		places[i]->setDist(FindDist("", to_string(places[i]->getID()), to_string(endNode), false));
		places[i]->setFound(false);
		if (places[i]->getID() == startNode)
		{
			start = places[i];
			start->setFound(true);
			Q.push_back(start);
		}
		else if (places[i]->getID() == endNode)
		{
			end = places[i];
		}
	}
	while (Q.size() > 0)
	{
		v = Q[Q.size() - 1];
		Q.pop_back();
		if (v == end)
		{
			nextNode = v;
			routes.push_back(nextNode);
			while (nextNode != start) 
			{	
				nextNode = nextNode->getParent();
				routes.push_back(nextNode);
			}
			found = true;
		}
		else {
			for (int x = 0; x < v->arcSize(); x++)
			{
				if (!v->getArc(x)->getEnd()->checkFound())
				{
					for (int y = 0; y < v->getArc(x)->modesSize(); y++)
					{
						if (v->getArc(x)->getMode(y) == mode)
						{
							v->getArc(x)->getEnd()->setFound(true);
							v->getArc(x)->getEnd()->setParent(v);
							Q.push_back(v->getArc(x)->getEnd());
						}
					}
				}
			}
		}
	}
	if (!found) 
	{
		_outFile << "Fail" << endl;
	}
	
	const size_t x = routes.size();

	for (int i = 0; i < x; i++) 
	{
		_outFile << routes[routes.size()-1]->getID() << endl;
		routes.pop_back();
	}

	_outFile << endl;
}