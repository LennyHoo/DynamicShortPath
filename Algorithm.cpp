#include "Algorithm.h"
#include "Edge.h"
#include "QDebug"
#include "QMessageBox"

#define INF INT_MAX

void ShortestWaySolution:: solve(QVector<Vertex*> *V, QVector<Edge> *E, Vertex *start, Vertex *end){
    /*
        ���·�� ������㷨
    */
    // ��ʼ��
    int n = V->size();
    for(auto v=V->begin(); v!=V->end(); ++v){
        if(*v==start) continue; // Դ�㲻��·������
        bool isInf = true;
        for(auto e=E->begin(); e!=E->end(); ++e){
            if(e->edge.first==start&&e->edge.second==*v){
                QStack<Edge*> *s = new QStack<Edge*>;
                s->push(e);
                paths->insert(*v, *s);
                isInf = false;
                dist->insert(*v, e->weight);
            }
        }
        if(isInf){
            QStack<Edge*> *s = new QStack<Edge*>;
            Edge *e = new Edge(start, *v, INF);
            s->push(e);
            paths->insert(*v, *s);
            dist->insert(*v, INF);
        }
    }
    // ����������
    while(solved->size()<n-2){
        // �ҵ���ǰ��ȷ�������·��
        int min_distance = INF;
        Vertex *min_vertex = nullptr;
        for(auto d=dist->begin(); d!=dist->end(); ++d){
            if(d.value()<min_distance&&solved->indexOf(d.key())==-1){
                min_distance = d.value();
                min_vertex = d.key();
            }
        }
        // �� ��ȷ�� �ҵ����·����һ�������� �ѽ����� �ļ�����
        solved->push_back(min_vertex);
        for(auto e=E->begin(); e!=E->end(); ++e){
            if(e->edge.first==min_vertex){
                auto d = dist->find(e->edge.second);
                if(min_distance+e->weight<d.value()){
                    d.value() = min_distance + e->weight;
                    auto path = paths->find(e->edge.second);
                    auto pre_min_path = paths->find(min_vertex);
                    path.value() = pre_min_path.value();
                    path.value().push(e);
                }
            }
        }
    }
    auto path = paths->find(end);
    auto distance = dist->find(end);
    QVector<Edge*> p = path.value();
    for(auto e=E->begin(); e!=E->end(); ++e)
        e->chosed = false;
    if(distance.value()==INF)
         QMessageBox::information(nullptr, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("������ͨ·"));
    else
        QMessageBox::information(nullptr, QString::fromLocal8Bit("��ʾ"),
                                 QString::fromLocal8Bit("���·�����ȣ�")+QString::number(distance.value()));
    for(int i=0; i<p.size(); ++i)
            p.at(i)->chosed = true;
    emit shortest_way_found();
}

ShortestWaySolution:: ~ShortestWaySolution(){

};
