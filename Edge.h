#ifndef EDGE_H
#define EDGE_H

#include "QVector"
#include "Vertex.h"
#include "QLabel"

class Edge{
public:
    pair<Vertex*, Vertex*> edge;
    int weight = INT_MAX;
    bool chosed = false;
public:
    Edge(Vertex* from, Vertex* to, int weight);
    Edge(pair<Vertex*, Vertex*> edge, int weight);
    virtual ~Edge();
};


#endif // EDGE_H
