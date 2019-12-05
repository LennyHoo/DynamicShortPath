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
        ������ʽ
    */
    this->setStyleSheet(OnSheetStyle);
}

void Vertex:: mouseMoveEvent(QMouseEvent *event){
    /*
        �����ק�¼�
    */
    if (event->buttons() & Qt::LeftButton)
        {
            //�ƶ��е����λ������ڳ�ʼλ�õ����λ��.
            QPoint relativePos = event->globalPos() - m_point;
            //Ȼ���ƶ����弴��.
            this->move(m_pos + relativePos);
        }


        QWidget::mouseMoveEvent(event);
}

void Vertex:: mousePressEvent(QMouseEvent *event){
    /*
        �����������¼�
        ��¼������������.
        ��¼�������������.
    */
    if (event->button() == Qt::LeftButton)
       {

           //��¼������������.
           m_point = event->globalPos();
           //��¼�������������.
           m_pos = this->frameGeometry().topLeft();
       }
       QWidget::mousePressEvent(event);
}

void Vertex:: mouseDoubleClickEvent(QMouseEvent * event){
    /*
        ˫��ѡ��һ������.
        ����Ѿ�ѡ����������һ����������һ����.
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
        �л� ���� �� ѡ�� �� �ɿ� ��״̬��ʾ
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
