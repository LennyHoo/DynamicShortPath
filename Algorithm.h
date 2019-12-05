#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Vertex.h"
#include "Edge.h"
#include "QMap"
#include "QStack"

class ShortestWaySolution: public QObject{
    Q_OBJECT
public:
    Vertex* start_point;
    QMap<Vertex*, QStack<Edge*>> *paths = new QMap<Vertex*, QStack<Edge*>>;
    QMap<Vertex*, int> *dist = new QMap<Vertex*, int>;
    QVector<Vertex*> *solved = new QVector<Vertex*>;
public:
    void solve(QVector<Vertex*> *V, QVector<Edge> *E, Vertex *start, Vertex *end);
    virtual ~ShortestWaySolution();
signals:
    void shortest_way_found();
};

#endif // ALGORITHM_H
