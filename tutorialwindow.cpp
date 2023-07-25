/***********************************************************************
 * Author:  B11115010 鍾詩靈
 *          B11115014 魏美芳
 * Create Date: 2023/05/24
 * Update Date: 2023/06/01
 * Description: This C++ program performs dungeon game with kirby theme that have many features (see readme file)
***********************************************************************/

#include "tutorialwindow.h"
#include "ui_tutorialwindow.h"

// Intent: Initialize the TutorialWindow widget with the specified parent.
// Pre: The parent widget must be a valid QWidget.
// Post: The TutorialWindow widget is created and set up with the UI.
TutorialWindow::TutorialWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TutorialWindow)
{
    // Set up the UI
    ui->setupUi(this);
}

// Intent: Destructor for the TutorialWindow widget.
// Pre: None.
// Post: The TutorialWindow widget is destroyed and memory is freed.
TutorialWindow::~TutorialWindow()
{
    delete ui;
}

// Intent: Handle the button click event for the "Back" button.
// Pre: None.
// Post: Emit a signal to notify the parent widget to go back to the menu.
void TutorialWindow::on_back_btn_clicked()
{
    emit BackToMenuClicked();
}

