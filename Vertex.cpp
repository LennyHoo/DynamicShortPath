#include "Vertex.h"
#include "QPoint"
#include "QCursor"
#include "QMouseEvent"
#include "QDebug"
#include "QtMath"
#include "widget.h"

stack<Vertex*> chosed;


Vertex:: Vertex(){
    /*
        设置样式
    */
    this->setStyleSheet(OnSheetStyle);
}

void Vertex:: mouseMoveEvent(QMouseEvent *event){
    /*
        鼠标拖拽事件
    */
    if (event->buttons() & Qt::LeftButton)
        {
            //移动中的鼠标位置相对于初始位置的相对位置.
            QPoint relativePos = event->globalPos() - m_point;
            //然后移动窗体即可.
            this->move(m_pos + relativePos);
        }


        QWidget::mouseMoveEvent(event);
}

void Vertex:: mousePressEvent(QMouseEvent *event){
    /*
        鼠标左键按下事件
        记录鼠标的世界坐标.
        记录窗体的世界坐标.
    */
    if (event->button() == Qt::LeftButton)
       {

           //记录鼠标的世界坐标.
           m_point = event->globalPos();
           //记录窗体的世界坐标.
           m_pos = this->frameGeometry().topLeft();
       }
       QWidget::mousePressEvent(event);
}

void Vertex:: mouseDoubleClickEvent(QMouseEvent * event){
    /*
        双击选中一个顶点.
        如果已经选择了其他的一个顶点则作一条弧.
    */
    if (event->button() == Qt::LeftButton)
       {
            switch(this->SheetStyle){
            case On:
                this->setStyleSheet(DownSheetStyle);
                this->SheetStyle = Down;
                chosed.push(this);
                if(chosed.size()==2)
                    emit add_new_edge();
                break;
            case Down:
                this->setStyleSheet(OnSheetStyle);
                this->SheetStyle = On;
                chosed.pop();
                break;
            }
       }
       QWidget::mousePressEvent(event);
}

void Vertex:: ToggleSheetStyle(){
    /*
        切换 顶点 被 选中 和 松开 的状态显示
    */
    switch(this->SheetStyle){
    case On:
        this->setStyleSheet(DownSheetStyle);
        this->SheetStyle = Down;
        break;
    case Down:
        this->setStyleSheet(OnSheetStyle);
        this->SheetStyle = On;
        break;
    }
}
