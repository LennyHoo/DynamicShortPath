#include "Edge.h"

Edge::Edge(Vertex* from, Vertex* to, int weight){
     /*
        ��ʼ�� ��β ��ͷ Ȩֵ
     */
    this->edge.first = from;
    this->edge.second = to;
    this->weight = weight;
}

Edge::Edge(pair<Vertex*, Vertex*> edge, int weight){
    /*
       ��ʼ�� ��β ��ͷ Ȩֵ
    */
    this->edge = edge;
    this->weight = weight;
}

Edge::~Edge(){

}
