#include "Arc.h"

Arc::Arc(Node* const Destination, const double Distance, const vector<string> &Vehicles) : m_vehicles(Vehicles), m_destination(Destination), m_distance(Distance)
{
}

Arc::Arc(void) : m_destination(nullptr), m_distance(0)
{
}