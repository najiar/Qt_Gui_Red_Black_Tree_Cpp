#include "renderarea.h"
#include <QApplication>
#include <QPainter>
#include <qglobal.h>
#include <QTime>

RenderArea::RenderArea(RBTree *RBT, QWidget *parent) : QWidget(parent), RBT(), scale(1.0)
{
    this->RBT = RBT;
    this->scale = 1; //default of the scale
}
// constructor with parameters

QSize RenderArea::sizeHint() const
{
    return QSize(50, 50);
}
// sizeHint

QSize RenderArea::minimumSizeHint() const
{
    return QSize(50, 50);
}
// minimumSizeHint

// What to do when the render area gets repaint() called
void RenderArea::paintEvent(QPaintEvent *)
{
  // Only repaint the tree if it's not empty
  if (this->RBT->is_empty())
  {
      return;
  }

  QPainter painter(this);
  this->RBT->draw(&painter, this->scale); //draw the tree on repaint()
  this->areaResize(); //resize
}
//paintEvent

// Increment the scale variable and redraw
void RenderArea::areaZoomIn()
{
    if (this->RBT->is_empty())
    {
        return;
    }
    if(this->scale < 2.0) //if the scale is smaller than max
    {
        this->scale += 0.1; //increment the zoomin
        this->repaint();
    }
}
// areaZoomIn

// Decrement the scale variable and redraw
void RenderArea::areaZoomOut()
{
    if (this->RBT->is_empty())
    {
        return;
    }
    if(this->scale > 0.2) //if the scale is bigger than min
    {
        this->scale -= 0.1; //decrement the scale
        this->repaint();
    }
}
// areaZoomOut

// Handle mouse clicking that is done on the QScrollArea that should
// be handled by the RenderArea (for areaZoomIng)
bool RenderArea::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *mb = static_cast<QMouseEvent *>(event);
        switch(mb->button())
        {
        case Qt::LeftButton: //in case left button is clicked
            this->areaZoomIn(); //call zoom in
            break;
        case Qt::RightButton://in case right button is clicked
            this->areaZoomOut(); //call zoom out
            break;
        default:
            return true;
        }
    }
    return true;
}
// eventFilter

//Auto size the render area based on the required size by the tree
void RenderArea::areaResize()
{
    //resize the tree depends on get_y and get_x
    QSize size(RBT->get_x(), RBT->get_y());
    this->setMinimumSize(size);
    this->resize(size);
}
// areaResize
