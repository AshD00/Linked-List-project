#pragma once

#include <string>
#include <vector>

using namespace std;
class Arc;

class Node
{
private:
	string m_name = "";
	vector<Arc*> arcs;
	Node* m_parentNode = nullptr;
	int m_id = 0;
	int m_gval = 0;
	int m_fval = 0;
	double m_x = 0, m_y = 0, m_distance = 0;
	bool m_discovered = false;

	Node(const Node& obj) : m_name(obj.m_name), arcs(obj.arcs), m_parentNode(obj.m_parentNode),  m_id(obj.m_id), m_x(obj.m_x), m_y(obj.m_y), m_distance(obj.m_distance), m_discovered(obj.m_discovered) {
	}

	Node& operator = (const Node& obj) {
		if (&obj != this) {
			arcs = obj.arcs;
			m_discovered = obj.m_discovered;
			m_distance = obj.m_distance;
			m_id = obj.m_id;
			m_name = obj.m_name;
			m_parentNode = obj.m_parentNode;
			m_x = obj.m_x;
			m_y = obj.m_y;
			m_fval = obj.m_fval;
			m_gval = obj.m_gval;
		}
		return *this;
	}
public:
	const string &getName() const {
		return m_name;
	}
	int getID() const {
		return m_id;
	}
	double getX() const {
		return m_x;
	}
	double getY() const {
		return m_y;
	}
	double getDist() const {
		return m_distance;
	}
	int getG() const {
		return m_gval;
	}
	int getF() const {
		return m_fval;
	}
	bool checkFound() const {
		return m_discovered;
	}
	Node* getParent() const {
		return m_parentNode;
	}
	Arc const* getArc(const int pos) {
		return arcs[pos];
	}
	size_t arcSize() const {
		return arcs.size();
	}

	string setName(const string& name) {
		m_name = name;
	}
	void setID(const int ID) {
		m_id = ID;
	}
	void setX(const double x) {
		m_x = x;
	}
	void setY(const double y) {
		m_y = y;
	}
	void setDist(const double dist) {
		m_distance = dist;
	}
	void setG(const int g) {
		m_gval = g;
	}
	void setF(const int f) {
		m_fval = f;
	}
	void setFound(const bool discovered) {
		m_discovered = discovered;
	}
	void setParent(Node* const parent) {
		m_parentNode = parent;
	}
	void createArc(Arc* const newArc) {
		arcs.push_back(newArc);
	}

	Node(const string &Name, int ID, double X, double Y);
	Node(void);
};