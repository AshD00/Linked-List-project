#pragma once

#include <string>
#include <vector>

using namespace std;
class Node;

class Arc
{
private:
	vector<string> m_vehicles;
	Node* m_destination;
	double m_distance;

	Arc(const Arc& obj) :  m_vehicles(obj.m_vehicles), m_destination(obj.m_destination), m_distance(obj.m_distance) {
	}

	Arc& operator = (const Arc& obj) {
		if (&obj != this) {
			m_destination = obj.m_destination;
			m_vehicles = obj.m_vehicles;
			m_distance = obj.m_distance;
		}
		return *this;
	}
public:
	// (1) Foot, (2) Bike, (3) Car, (4) Bus, (5) Rail, (6) Ship
	Node* getEnd()  const {
		return m_destination;
	}
	double getDist() const {
		return m_distance;
	}
	string getMode(const int pos) const {
		return m_vehicles[pos];
	}
	const size_t modesSize() const {
		return m_vehicles.size();
	}
	void setEnd(Node* const endNode) {
		m_destination = endNode;
	}
	void setDist(const double distance) {
		m_distance = distance;
	}
	void createMode(const string& vehicle) {
		m_vehicles.push_back(vehicle);
	}

	Arc(Node* m_destination, const double m_distance, const vector<string> &m_vehicles);
	Arc(void);
};