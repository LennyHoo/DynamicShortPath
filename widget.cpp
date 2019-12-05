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
        画箭头函数
    */
    double par = 15.0;//箭头部分三角形的腰长
    double slopy = atan2((endPoint.y() - startPoint.y()), (endPoint.x() - startPoint.x()));
    double cosy = cos(slopy);
    double siny = sin(slopy);
    QPoint point1 = QPoint(endPoint.x() + int(-par*cosy - (par / 2.0*siny)), endPoint.y() + int(-par*siny + (par / 2.0*cosy)));
    QPoint point2 = QPoint(endPoint.x() + int(-par*cosy + (par / 2.0*siny)), endPoint.y() - int(par / 2.0*cosy + par*siny));
    QPoint points[3] = { endPoint, point1, point2 };
    p.setRenderHint(QPainter::Antialiasing, true);//消锯齿
    QPen drawTrianglePen;//创建画笔
    if(chosed)
        drawTrianglePen.setColor(Qt::green);
    else{
        drawTrianglePen.setColor(Qt::blue);
    }
    drawTrianglePen.setWidth(1);
    p.setPen(drawTrianglePen);
    p.drawPolygon(points, 3);//绘制箭头部分
    int offsetX = int(par*siny / 3);
    int offsetY = int(par*cosy / 3);
    QPoint point3, point4;
    point3 = QPoint(endPoint.x() + int(-par*cosy - (par / 2.0*siny)) + offsetX, endPoint.y() + int(-par*siny + (par / 2.0*cosy)) - offsetY);
    point4 = QPoint(endPoint.x() + int(-par*cosy + (par / 2.0*siny) - offsetX), endPoint.y() - int(par / 2.0*cosy + par*siny) + offsetY);
    QPoint arrowBodyPoints[3] = { startPoint, point3, point4 };
    p.drawPolygon(arrowBodyPoints, 3);//绘制箭身部分
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 设置窗口大小和标题
    this->resize(800, 600);
    this->setWindowTitle(QString:: fromLocal8Bit("动态最短路径"));
    // “添加结点” 按钮
    QPushButton *add_v_btn = new QPushButton;
    add_v_btn->setParent(this);
    add_v_btn->setText(QString::fromLocal8Bit("添加"));
    add_v_btn->resize(60, 30);
    add_v_btn->move(135, 5);
    // “删除结点” 按钮
    QPushButton *delete_v_btn = new QPushButton;
    delete_v_btn->setParent(this);
    delete_v_btn->setText(QString::fromLocal8Bit("删除"));
    delete_v_btn->resize(60, 30);
    delete_v_btn->move(200, 5);
    // 起点 标签
    QLabel *start_label = new QLabel;
    start_label->setParent(this);
    start_label->setText(QString::fromLocal8Bit("从"));
    start_label->resize(30, 30);
    start_label->move(5, 40);
    start_label->show();
    // 行内文本框， 用于输入起点
    QLineEdit *start_v_edit = new QLineEdit;
    start_v_edit->setParent(this);
    start_v_edit->resize(80, 30);
    start_v_edit->move(25, 40);
    // 终点标签
    QLabel *end_label = new QLabel;
    end_label->setParent(this);
    end_label->setText(QString::fromLocal8Bit("到"));
    end_label->resize(30, 30);
    end_label->move(120, 40);
    end_label->show();
    // 行内文本框， 用于输入终点
    QLineEdit *end_v_edit = new QLineEdit;
    end_v_edit->setParent(this);
    end_v_edit->resize(80, 30);
    end_v_edit->move(140, 40);
    // 推荐线路 按钮
    QPushButton *short_path_button = new QPushButton;
    short_path_button->setParent(this);
    short_path_button->setText(QString::fromLocal8Bit("推荐路线"));
    short_path_button->resize(80, 30);
    short_path_button->move(230, 40);
    // 行内文本框， 用于输入新添加的结点的名称
    QLineEdit *v_name_edit = new QLineEdit;
    v_name_edit->setParent(this);
    v_name_edit->setPlaceholderText(QString::fromLocal8Bit("结点名称"));
    v_name_edit->resize(120, 30);
    v_name_edit->move(5, 5);
    // 按下 “添加结点” 按钮， 新建一个结点
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
    // 按下 “删除结点” 按钮， 删除一个结点
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
    // 按下 推荐路线 按钮 找寻 最佳路径
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
        储存图中的边，用于绘制
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
    QString weight = QInputDialog::getText(nullptr, QString::fromLocal8Bit("权值输入"),
                                                           QString::fromLocal8Bit("请输入这条边的权重"),
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

