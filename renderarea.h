#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QMouseEvent>
#include "RBTree.h"
#include <QColor>

class RenderArea : public QWidget   // inherits from QWidget
{
public:
    RenderArea(RBTree *RBT, QWidget *parent = 0);   // constructor

    QSize minimumSizeHint() const override; // ensures that the widget won't get smaller than minimum size
    QSize sizeHint() const override;    // used when the widget is created to give it a proper size
    void areaZoomIn();  // zooms in
    void areaZoomOut(); // zooms out
    bool eventFilter(QObject *, QEvent *event) override;    // used to get the mouse clicking event
    void areaResize();  // resizes the renderArea

protected:
    void paintEvent(QPaintEvent *event) override;   // called on repaint
private:
    RBTree *RBT;    // pointer of type RBT
    Node *node; // pointer to node

    double scale;   // scale
};

#endif // RENDERAREA_H
