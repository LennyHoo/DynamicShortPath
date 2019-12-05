#include "widget.h"
#include "ui_widget.h"
#include "QPushButton"
#include "QString"
#include "Vertex.h"
#include "QMessageBox"
#include "QVector"
#include "QLineEdit"
#include "QPainter"
#include "QtMath"
#include "QDebug"
#include "QInputDialog"
#include "Algorithm.h"

using namespace std;

extern stack<Vertex*> chosed;

void drawArrow(QPoint startPoint, QPoint endPoint, QPainter &p, bool chosed)
{
    /*
        ����ͷ����
    */
    double par = 15.0;//��ͷ���������ε�����
    double slopy = atan2((endPoint.y() - startPoint.y()), (endPoint.x() - startPoint.x()));
    double cosy = cos(slopy);
    double siny = sin(slopy);
    QPoint point1 = QPoint(endPoint.x() + int(-par*cosy - (par / 2.0*siny)), endPoint.y() + int(-par*siny + (par / 2.0*cosy)));
    QPoint point2 = QPoint(endPoint.x() + int(-par*cosy + (par / 2.0*siny)), endPoint.y() - int(par / 2.0*cosy + par*siny));
    QPoint points[3] = { endPoint, point1, point2 };
    p.setRenderHint(QPainter::Antialiasing, true);//�����
    QPen drawTrianglePen;//��������
    if(chosed)
        drawTrianglePen.setColor(Qt::green);
    else{
        drawTrianglePen.setColor(Qt::blue);
    }
    drawTrianglePen.setWidth(1);
    p.setPen(drawTrianglePen);
    p.drawPolygon(points, 3);//���Ƽ�ͷ����
    int offsetX = int(par*siny / 3);
    int offsetY = int(par*cosy / 3);
    QPoint point3, point4;
    point3 = QPoint(endPoint.x() + int(-par*cosy - (par / 2.0*siny)) + offsetX, endPoint.y() + int(-par*siny + (par / 2.0*cosy)) - offsetY);
    point4 = QPoint(endPoint.x() + int(-par*cosy + (par / 2.0*siny) - offsetX), endPoint.y() - int(par / 2.0*cosy + par*siny) + offsetY);
    QPoint arrowBodyPoints[3] = { startPoint, point3, point4 };
    p.drawPolygon(arrowBodyPoints, 3);//���Ƽ�����
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // ���ô��ڴ�С�ͱ���
    this->resize(800, 600);
    this->setWindowTitle(QString:: fromLocal8Bit("��̬���·��"));
    // ����ӽ�㡱 ��ť
    QPushButton *add_v_btn = new QPushButton;
    add_v_btn->setParent(this);
    add_v_btn->setText(QString::fromLocal8Bit("���"));
    add_v_btn->resize(60, 30);
    add_v_btn->move(135, 5);
    // ��ɾ����㡱 ��ť
    QPushButton *delete_v_btn = new QPushButton;
    delete_v_btn->setParent(this);
    delete_v_btn->setText(QString::fromLocal8Bit("ɾ��"));
    delete_v_btn->resize(60, 30);
    delete_v_btn->move(200, 5);
    // ��� ��ǩ
    QLabel *start_label = new QLabel;
    start_label->setParent(this);
    start_label->setText(QString::fromLocal8Bit("��"));
    start_label->resize(30, 30);
    start_label->move(5, 40);
    start_label->show();
    // �����ı��� �����������
    QLineEdit *start_v_edit = new QLineEdit;
    start_v_edit->setParent(this);
    start_v_edit->resize(80, 30);
    start_v_edit->move(25, 40);
    // �յ��ǩ
    QLabel *end_label = new QLabel;
    end_label->setParent(this);
    end_label->setText(QString::fromLocal8Bit("��"));
    end_label->resize(30, 30);
    end_label->move(120, 40);
    end_label->show();
    // �����ı��� ���������յ�
    QLineEdit *end_v_edit = new QLineEdit;
    end_v_edit->setParent(this);
    end_v_edit->resize(80, 30);
    end_v_edit->move(140, 40);
    // �Ƽ���· ��ť
    QPushButton *short_path_button = new QPushButton;
    short_path_button->setParent(this);
    short_path_button->setText(QString::fromLocal8Bit("�Ƽ�·��"));
    short_path_button->resize(80, 30);
    short_path_button->move(230, 40);
    // �����ı��� ������������ӵĽ�������
    QLineEdit *v_name_edit = new QLineEdit;
    v_name_edit->setParent(this);
    v_name_edit->setPlaceholderText(QString::fromLocal8Bit("�������"));
    v_name_edit->resize(120, 30);
    v_name_edit->move(5, 5);
    // ���� ����ӽ�㡱 ��ť�� �½�һ�����
    connect(add_v_btn, &QPushButton:: clicked, this, [=](){
        Vertex *v = new Vertex;
        v->name = v_name_edit->text();
        v->setText(v->name);
        v->setParent(this);
        v->move((this->width()-v->width())/2, (this->height()-v->height())/2);
        v->show();
        v_name_edit->setText(QString::fromLocal8Bit(""));
        connect(v, &Vertex:: add_new_edge, this, &Widget::draw_edges);
        V->push_back(v);
    });
    // ���� ��ɾ����㡱 ��ť�� ɾ��һ�����
    connect(delete_v_btn, &QPushButton:: clicked, this, [&](){
        if(chosed.empty())  return;
        Vertex* &v = chosed.top();
        chosed.pop();
        for(int i=0; i<E->size(); ++i){
            if(E->at(i).edge.first==v||E->at(i).edge.second==v){
                E->removeAt(i);
                --i;
            }
        }
        update();
        for(int i=0; i<V->size(); ++i){
            if(V->at(i)==v)
                V->removeAt(i);
        }
        v->hide();
    });
    // ���� �Ƽ�·�� ��ť ��Ѱ ���·��
    connect(short_path_button, &QPushButton::clicked, this, [=](){
        ShortestWaySolution *solution = new ShortestWaySolution;
        connect(solution, &ShortestWaySolution::shortest_way_found, this, &Widget:: highlight_shortest_way);
        QString start_point_name = start_v_edit->text();
        QString end_point_name = end_v_edit->text();
        int start_point_index = LocateVertex(start_point_name);
        int end_point_index = LocateVertex(end_point_name);
        if(start_point_index!=-1&&end_point_index!=-1){
            solution->solve(V, E, V->at(start_point_index), V->at(end_point_index));
        }
    });
}

void Widget:: draw_edges(){
    /*
        ����ͼ�еıߣ����ڻ���
    */
    Vertex *head = chosed.top();
    chosed.pop();
    Vertex *tail = chosed.top();
    chosed.pop();
    pair<Vertex*, Vertex*> new_edge = pair<Vertex*, Vertex*>(tail, head);
    for(auto it=E->begin(); it!=E->end(); ++it){
        if(it->edge==new_edge){
            E->erase(it);
            tail->ToggleSheetStyle();
            head->ToggleSheetStyle();
            update();
            return;
        }
    }
    bool isOK;
    QString weight = QInputDialog::getText(nullptr, QString::fromLocal8Bit("Ȩֵ����"),
                                                           QString::fromLocal8Bit("�����������ߵ�Ȩ��"),
                                                           QLineEdit::Normal,
                                                           "",
                                                           &isOK);
    if(isOK){
        E->push_back(Edge(new_edge, weight.toInt()));
    }
    tail->ToggleSheetStyle();
    head->ToggleSheetStyle();
    update();
}

void Widget:: highlight_shortest_way(){
    update();
}

void Widget:: paintEvent(QPaintEvent *event){
    QPainter painter(this);
    for(auto it=E->begin(); it!=E->end(); ++it){
        QPoint from = it->edge.first->pos(), to = it->edge.second->pos();
        float r = it->edge.first->width()/2;
        from.setX(static_cast<int>(from.x()+r));
        from.setY(static_cast<int>(from.y()+r));
        to.setX(static_cast<int>(to.x()+r));
        to.setY(static_cast<int>(to.y()+r));
        float distance = sqrtf(powf(from.x()-to.x(), 2)+powf(from.y()-to.y(), 2));
        float sina = (from.y()-to.y())/distance;
        float cosa = (from.x()-to.x())/distance;
        float beta = 32.f * 3.14f / 180.f;
        sina = sina*cos(beta) + cosa*sin(beta);
        cosa = cosa*cos(beta) - sina*sin(beta);
        from.setX(static_cast<int>(from.x()+r*cosa));
        from.setY(static_cast<int>(from.y()+r*sina));
        to.setX(static_cast<int>(to.x()+r*cosa));
        to.setY(static_cast<int>(to.y()+r*sina));
        drawArrow(from, to, painter, it->chosed);
        QPoint weight_pos = QPoint((from.x()+to.x())/2, (from.y()+to.y())/2);
        painter.setPen(Qt::black);
        QFont font;
        font.setPointSize(16);
        painter.setFont(font);
        painter.drawText(weight_pos, QString::number(it->weight));
    }
};

int Widget:: LocateVertex(QString s){
    for(int i=0; i<V->size(); ++i){
       if(V->at(i)->name==s)
           return i;
    }
    return -1;
}

Widget::~Widget()
{
    delete ui;
}

