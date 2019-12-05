#pragma once

#include "QPushButton"
#include "QString"
#include "QPoint"
#include "QPainter"
#include "stack"

using namespace std;

typedef enum{On, Down} V_SheetStyle_t;


class Vertex: public QPushButton{
    Q_OBJECT
public:
    QString name;
    QString intro;
    QPoint m_pos;
    QPoint m_point;
    const QString OnSheetStyle = "min-width: 64px; min-height: 64px;"
                               "max-width: 64px; max-height: 64px;"
                               "border-radius: 32px;  "
                               "border:2px solid black;"
                               "background: white";
    const QString DownSheetStyle = "min-width: 64px; min-height: 64px;"
                               "max-width: 64px; max-height: 64px;"
                               "border-radius: 32px;  "
                               "border:2px solid black;"
                               "background: gray";
    V_SheetStyle_t SheetStyle = On;
signals:
    void add_new_edge();
public:
    Vertex();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void ToggleSheetStyle();
};

