#include "Edge.h"

Edge::Edge(Vertex* from, Vertex* to, int weight){
     /*
        初始化 弧尾 弧头 权值
     */
    this->edge.first = from;
    this->edge.second = to;
    this->weight = weight;
}

Edge::Edge(pair<Vertex*, Vertex*> edge, int weight){
    /*
       初始化 弧尾 弧头 权值
    */
    this->edge = edge;
    this->weight = weight;
}

Edge::~Edge(){

}
