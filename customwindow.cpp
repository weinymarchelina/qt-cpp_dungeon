/***********************************************************************
 * Author:  B11115010 鍾詩靈
 *          B11115014 魏美芳
 * Create Date: 2023/05/24
 * Update Date: 2023/06/01
 * Description: This C++ program performs dungeon game with kirby theme that have many features (see readme file)
***********************************************************************/

#include "customwindow.h"
#include "ui_customwindow.h"

// Intent: Initialize TutorialWindow
// Pre: None
// Post: TutorialWindow is initialized with the specified parent and UI
CustomWindow::CustomWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomWindow)
{
    // Set up the UI for TutorialWindow
    ui->setupUi(this);
}

// Intent: Destroy TutorialWindow
// Pre: None
// Post: TutorialWindow is destroyed and memory is freed
CustomWindow::~CustomWindow()
{
    // Delete the UI of TutorialWindow
    delete ui;
}

// Intent: Handle back button click
// Pre: None
// Post: Emit the signal BackToMenuClicked to indicate that the back button was clicked
void CustomWindow::on_back_btn_clicked()
{
    // Emit the BackToMenuClicked signal
    emit BackToMenuClicked();
}

// Intent: Reset spin box values
// Pre: None
// Post: The spin box values are reset to their default values
void CustomWindow::resetSpinBox() {
    // Reset the values of each spin box to their default values
    ui->spinBox_dee->setValue(1);
    ui->spinBox_scarfy->setValue(0);
    ui->spinBox_bomber->setValue(0);
    ui->spinBox_width->setValue(5);
    ui->spinBox_height->setValue(5);
    ui->spinBox_trigger->setValue(1);
    ui->spinBox_food->setValue(0);
    ui->spinBox_tomato->setValue(0);
    ui->error_text->hide();
}

// Intent: Handle play button click
// Pre: None
// Post: Validate the custom board settings and emit the StartGameClicked signal if valid
void CustomWindow::on_play_btn_clicked()
{
    // Clear the custom properties
    customProperties.clear();

    // Set the initial validity flag to true
    bool validBoard = true;
    QString msg = "";

    // Get the values from the spin boxes
    int deeCount = ui->spinBox_dee->value();
    int scarfyCount = ui->spinBox_scarfy->value();
    int bomberCount = ui->spinBox_bomber->value();
    int boardWidth = ui->spinBox_width->value();
    int boardHeight = ui->spinBox_height->value();
    int triggerCount = ui->spinBox_trigger->value();
    int foodCount = ui->spinBox_food->value();
    int tomatoRate = ui->spinBox_tomato->value();

    // Calculate the total number of objects on the board
    int totalObj = deeCount + scarfyCount + bomberCount + (triggerCount * 2) + foodCount + 1;

    // Check if the total number of objects exceeds the available space on the board
    if (totalObj > ((boardWidth - 2) * (boardHeight - 2))) {
        validBoard = false;
        msg = "Please enter wider board size to store enough objects";
    }

    // Check if any of the input values are invalid
    if (deeCount < 0 || scarfyCount < 0 || bomberCount < 0 || foodCount < 0 || tomatoRate < 0 || triggerCount < 1 || boardWidth < 5 || boardHeight < 5) {
        validBoard = false;
        msg = "Please enter valid input (board size min. 5x5, must have at least 1 trigger)";
    }

    // Check if there are enough enemies on the board
    if (deeCount < 1 && scarfyCount < 1 && bomberCount < 1) {
        validBoard = false;
        msg = "Please enter valid input (board size min. 5x5, must have at least 1 enemy)";
    }

    // If the board settings are valid, store the custom properties and emit the StartGameClicked signal
    if (validBoard) {
        // Store the custom properties in the customProperties vector
        customProperties.push_back(boardWidth);
        customProperties.push_back(deeCount);
        customProperties.push_back(scarfyCount);
        customProperties.push_back(bomberCount);
        customProperties.push_back(triggerCount);
        customProperties.push_back(foodCount);
        customProperties.push_back(tomatoRate);
        customProperties.push_back(boardHeight);

         // Hide the error text
        ui->error_text->hide();

        // Emit the StartGameClicked signal
        emit StartGameClicked();
    }
    else {
         // Display the error message
        ui->error_text->show();
        ui->error_text->setText(msg);
    }
}

