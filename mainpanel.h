#ifndef mainpanel_H
#define mainpanel_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QColorDialog>
#include <QColor>
#include <QLineEdit>
#include "renderarea.h"
#include "RBTree.h"

class MainPanel : public QMainWindow
{
    Q_OBJECT
    QWidget *mainWidget;    // main window widget

public:
     MainPanel(QWidget *parent = 0);    // constructor
    ~MainPanel();   //destructor

private:
    RenderArea *renderArea; // pointer of class renderArea
    // Buttons
    QPushButton *deleteBtn;
    QPushButton *insertBtn;
    QPushButton *areaZoomInBtn;
    QPushButton *areaZoomOutBtn;

    // Controls
    QLineEdit *insertCtrl;
    QLineEdit *deleteCtrl;

    QScrollArea *scroll;    // provides scrolling view
    QVBoxLayout *Layout;    // main layout
    QLabel *updateLabel;    // status label to show status after each operation
    QMenu *fileDrop;    // file drop option
    QMenu *editDrop;    // edit drop option
    // Actions for the QMenu
    QAction *loadOption;
    QAction *saveOption;
    QAction *exitOption;
    QAction *resetOption;
    QAction *backgroundColourOption;

    RBTree *RBT;    // pointer to RBTree
    RBTree *getLastTree();  // gets the last saved tree in last_rbt
    void GenerateMenu();    // generates the QMenu
    void GenerateActions(); // binds the actions with functions

protected:
    virtual void closeEvent(QCloseEvent *event);    // what happens on close

private slots:
    void onInsert();    // gets inputed value and inserts into tree
    void onDelete() const;  // gets inputed value and deletes from tree
    void onAreaZoomIn() const;  // calls renderArea to zoom in
    void onAreaZoomOut() const; // calls renderArea to zoom out
    void loadFile();    // loads a tree from a file
    void saveFile();    // saves a tree to a file or as an image
    void exitProgram(); // exits the program
    void resetTree() const; // resets the tree
    void backgroundColour();    // changes the background color from the dialog of colors
};
#endif // mainpanel_H
