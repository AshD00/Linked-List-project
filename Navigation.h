#pragma once

#include <fstream>
#include <string>

#include "Node.h"
#include "Arc.h"


class Navigation
{
	std::ofstream _outFile;
	// Add your code here
	vector<Node*> places;
	
public:
	Navigation();
	~Navigation();

	bool BuildNetwork(const std::string& fileNamePlaces, const std::string& fileNameLinks);
	bool ProcessCommand(const std::string& commandString);

	// Add your code here

	void MaxDist(const std::string& commandString);
	void MaxLink(const std::string& commandString);
	double FindDist(const std::string& commandString, const string &startName, const string &endName, bool out);
	void FindNeighbour(const string& commandString, int node);
	void Check(const string& commandString, const string &mode, const vector<int> &nodes);
	void FindRoute(const string &mode, int startNode, int endNode);
	void ShortestRoute(const string &mode, int startNode, int endNode);
};