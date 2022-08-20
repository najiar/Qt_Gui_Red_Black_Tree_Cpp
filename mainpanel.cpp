#include "mainpanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCloseEvent>
#include <QMenuBar>
#include <qglobal.h>
#include <QTime>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QRegularExpression>


MainPanel::MainPanel(QWidget *parent) : QMainWindow(parent)
{
    // Create default save directory
    QString directory = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/RBT_GUI";
    if (!QDir(directory).exists())
    {
        QDir().mkdir(directory);
    }

    this->RBT = this->getLastTree(); //call the last rbt

    this->GenerateMenu(); //we call generatemenu to add actions to the drop options (edit, file)

    // Build buttons and layout for buttons on the bottom of the window

    deleteBtn = new QPushButton("Delete value", this);
    deleteBtn->setStyleSheet("font-size: 20px; background-color: red");

    insertBtn = new QPushButton("Insert value", this);
    insertBtn->setStyleSheet("font-size: 20px; background-color: green");
    areaZoomInBtn = new QPushButton("Zoom In", this);
    areaZoomInBtn->setStyleSheet("font-size: 20px;");
    areaZoomOutBtn = new QPushButton("Zoom Out", this);
    areaZoomOutBtn->setStyleSheet("font-size: 20px;");
    insertCtrl = new QLineEdit;
    deleteCtrl = new QLineEdit;
    updateLabel = new QLabel;

    // Set properties of buttons

    deleteBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    insertBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    areaZoomInBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    areaZoomOutBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    insertCtrl->setFixedWidth(300);
    insertCtrl->setFixedHeight(35);
    insertCtrl->setStyleSheet("font-size: 20px;");
    insertCtrl->setToolTip("Single value or multiple values (Separated by space) are accepted");

    deleteCtrl->setFixedWidth(100);
    deleteCtrl->setFixedHeight(35);
    deleteCtrl->setStyleSheet("font-size: 20px;");
    deleteCtrl->setToolTip("Value to be deleted");
    updateLabel->setStyleSheet("font-size: 20px;");

    // Connect the slots to the button signals

    connect(deleteBtn, SIGNAL(clicked()), this, SLOT(onDelete()));
    connect(insertBtn, SIGNAL(clicked()), this, SLOT(onInsert()));
    connect(areaZoomInBtn, SIGNAL(clicked()), this, SLOT(onAreaZoomIn()));
    connect(areaZoomOutBtn, SIGNAL(clicked()), this, SLOT(onAreaZoomOut()));
    connect(insertCtrl, SIGNAL(returnPressed()), this, SLOT(onInsert()));
    connect(deleteCtrl, SIGNAL(returnPressed()), this, SLOT(onDelete()));

    // Create button layout and add buttons
    QHBoxLayout *buttonCtrlLayout = new QHBoxLayout;

    buttonCtrlLayout->addWidget(insertBtn);
    buttonCtrlLayout->addWidget(insertCtrl);
    buttonCtrlLayout->addSpacing(30);
    buttonCtrlLayout->addWidget(deleteBtn);
    buttonCtrlLayout->addWidget(deleteCtrl);

    buttonCtrlLayout->addSpacing(25);
    buttonCtrlLayout->addWidget(updateLabel);
    buttonCtrlLayout->addStretch(0);
    buttonCtrlLayout->addWidget(areaZoomInBtn);
    buttonCtrlLayout->addWidget(areaZoomOutBtn);

    // Create the render area (canvas for drawing the RBT)
    renderArea = new RenderArea(this->RBT);

    scroll = new QScrollArea;
    scroll->setWidget(renderArea);

    // Pass any events that happen on the scroll area to the render area
    scroll->installEventFilter(renderArea);

    //add all the widgets to the main layout
    Layout = new QVBoxLayout;
    Layout->addWidget(scroll);
    Layout->addLayout(buttonCtrlLayout);

    // Build the main window
    mainWidget = new QWidget(this);
    mainWidget->setLayout(Layout);
    this->setCentralWidget(mainWidget);
    this->setMinimumHeight(500); //set the hegit size on manimaztion
    this->setWindowTitle("Red Black Tree GUI");
    this->showMaximized(); //show in full screen mode
}

MainPanel::~MainPanel()
{
    delete renderArea;
    delete deleteBtn;
    delete insertBtn;
    delete areaZoomInBtn;
    delete areaZoomOutBtn;
    delete scroll;
    delete RBT;
    delete mainWidget;
}

void MainPanel::GenerateMenu()
{
    this->GenerateActions(); //call the bindings

    fileDrop = this->menuBar()->addMenu(tr("&File"));
    fileDrop->addAction(loadOption);//add actions to File
    fileDrop->addAction(saveOption);
    fileDrop->addAction(exitOption);

    editDrop = this->menuBar()->addMenu(tr("&Edit"));
    editDrop->addAction(resetOption);//add actions to Edit
    editDrop->addAction(backgroundColourOption);
}

void MainPanel::GenerateActions()
{
    saveOption = new QAction(tr("&Save file"), this);
    saveOption->setStatusTip("Saves the tree");
    connect(saveOption, &QAction::triggered, this, &MainPanel::saveFile); //bind it with saveFile function

    exitOption = new QAction(tr("&Exit program"), this);
    exitOption->setStatusTip("Exits the program");
    connect(exitOption, &QAction::triggered, this, &MainPanel::exitProgram);//bind it with exitProgram function

    resetOption = new QAction(tr("&Reset Tree"), this);
    resetOption->setStatusTip("Deleted the tree");
    connect(resetOption, &QAction::triggered, this, &MainPanel::resetTree);//bind it with resetTree function

    backgroundColourOption = new QAction(tr("&Background color"), this);
    backgroundColourOption->setStatusTip("Change Background Color");
    connect(backgroundColourOption, &QAction::triggered, this, &MainPanel::backgroundColour); //bind it with backgroundColour function

    loadOption = new QAction(tr("&Load a file"), this);
    loadOption->setStatusTip("Load a tree from a file");
    connect(loadOption, &QAction::triggered, this, &MainPanel::loadFile); //bind it with loadfile function

}

void MainPanel::closeEvent(QCloseEvent *event)
{
    // Save RBTree before closing
    QString fileName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/RBT_GUI/last_RBT.txt"; //get to the last_rbt file

    QString text = RBT->get_preorder_trav(); //save the tree using get_preorder_trav function

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) //open it as write only
    {
        QTextStream writer(&file);
        writer << text; //output to the file
        writer.flush();
        file.close();
    }
    event->setAccepted(true); // set whether to close application or not
    return;
}

// Slot for delete button
void MainPanel::onDelete() const
{
    QString value = deleteCtrl->text(); // get the inpute value in the deleteCtrl

    if(!this->RBT->delete_value(value.toInt())) //delete it
    {
        this->updateLabel->setText("Value is not in tree..."); //update label
    }
    else
    {
        this->updateLabel->setText("Value deleted.");
    }

    this->renderArea->repaint(); // repaint to show changes to tree
    this->deleteCtrl->setText(""); // clear text box
    return;
}

// Slot for insert button
void MainPanel::onInsert()
{
    // Get entire line of text and iterate through the list of
    // values separated by whitespace - inserting all the values
    QString values = insertCtrl->text();

    QStringList valueList = values.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    QStringListIterator iter(valueList);

    while (iter.hasNext())
    {
        if(!this->RBT->insert(iter.next().toInt()))
        { // inserts 0 if text isn't an int
            this->updateLabel->setText("Please insert integer");
        }
        else
        {
            this->updateLabel->setText("The value is inserted");
        }
    }
    this->renderArea->repaint(); // repaint to show changes to tree
    insertCtrl->setText(""); // clear text box
    return;
}

// Slot for zoom in button
void MainPanel::onAreaZoomIn() const
{
    this->updateLabel->setText("");
    renderArea->areaZoomIn();
    return;
}

// Slot for zoom out button
void MainPanel::onAreaZoomOut() const
{
    this->updateLabel->setText("");
    renderArea->areaZoomOut();
    return;
}

// Slot for load in the file menu
void MainPanel::loadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                 QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/RBT_GUI",
                                 tr("Text files (*.txt)")); //get the file from the main directory

    QString text;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) //if we couldent open the file
    {
        this->updateLabel->setText("Could not open file!"); //update status
        return;
    }

    this->RBT->rbt_reset(); //reset the current tree

    QTextStream reader(&file);

    while (!reader.atEnd())//read the file
    {
        reader >> text; //input the read to string
        if(text != " " && text != "") //if its not empty
            this->RBT->insert(text.toInt()); //insert it into the tree
    }
    file.close(); //close the fail

    this->renderArea->repaint(); //repaind to apply changes

    this->updateLabel->setText("File successfully opened!"); //update status
    return;
}

// Slot for save action in menu
void MainPanel::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                 QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/RBT_GUI",
                                 tr("Text files (*.txt);;Images (*.png *.jpg)"));

    if (QFileInfo(fileName).suffix() == "txt") //if its txt
    {
        QString text = RBT->get_preorder_trav(); //get the current tree using get_preorder_trav
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            this->updateLabel->setText("File was not saved!");
            return;
        }
        QTextStream writer(&file);
        writer << text; //output to the file
        writer.flush();
        file.close();
        this->updateLabel->setText("File saved!");
        return;
    }

    // if not txt, save as image
    if(!this->renderArea->grab().save(fileName)) //if we want to save as image we use grab()
    {
        this->updateLabel->setText("Image was not saved");
        return;
    }
    this->updateLabel->setText("Image saved");

    return;
}

// Slot for exit action in menu
void MainPanel::exitProgram()
{
    this->close(); //close the program

    return;
}

// Slot for reset action in menu
void MainPanel::resetTree() const
{
    this->updateLabel->setText("Reset Applied");
    this->RBT->rbt_reset(); //call reset function
    this->renderArea->repaint();
    return;
}
// Slot for changing background color in menu
void MainPanel::backgroundColour()
{
    QColor color = QColorDialog::getColor(Qt::black, this); //open colour dialog
    if (color.isValid()) //if colour is valid
    {
        // change color
        QPalette pal(this->scroll->palette());
        pal.setColor(QPalette::Window, color); //set it to be the new color
        this->scroll->setPalette(pal);
    }
    return;
}

RBTree* MainPanel::getLastTree()
{

  RBTree *RBT = new RBTree; //we create new rbt

  QString fileName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/RBT_GUI/last_RBT.txt"; //we look for the last_rbt file

  QString text;
  QFile file(fileName);

  // If the file doesn't exist or if it can't open, return an empty RBT
  if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
      return RBT;
  }

  QTextStream reader(&file);


  while (!reader.atEnd()) //we read the file
  {

      reader >> text; //input to string
      if (text != " " && text != "")
          RBT->insert(text.toInt()); //we insert it to the newly created tree
  }

  file.close(); //close the file
  return RBT; //retrun the newly created rbt
}
