#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "QVector"
#include "Vertex.h"
#include "Edge.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
public:
    // 边 集合
    QVector<Edge> *E = new QVector<Edge>;
    // 结点 集合
    QVector<Vertex*> *V = new QVector<Vertex*>;
public:
    Widget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    int LocateVertex(QString s);
    ~Widget();
public slots:
    void draw_edges();
    void highlight_shortest_way();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
