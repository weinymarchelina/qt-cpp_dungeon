/***********************************************************************
 * Author:  B11115010 鍾詩靈
 *          B11115014 魏美芳
 * Create Date: 2023/05/24
 * Update Date: 2023/06/01
 * Description: This C++ program performs dungeon game with kirby theme that have many features (see readme file)
***********************************************************************/

#include "resultwindow.h"
#include "ui_resultwindow.h"

// Intent: Constructor for the ResultWindow class
// Pre: Takes a QWidget pointer as the parent
// Post: Initializes the ResultWindow and sets up the UI
ResultWindow::ResultWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWindow)
{
    // Set up the UI
    ui->setupUi(this);

    // Hide the medal and skin label initially
    ui->medal->hide();

    // Hide the skin label initially
    ui->skin_label->hide();
}

// Intent: Destructor for the ResultWindow class
// Pre: None
// Post: Deletes the ResultWindow UI
ResultWindow::~ResultWindow()
{
    delete ui;
}

// Intent: Handle the back button click event
// Pre: None
// Post: Emit the BackToMenuClicked signal
void ResultWindow::on_back_btn_clicked()
{
    emit BackToMenuClicked();
}

// Intent: Set the result of the game
// Pre: Takes a boolean value indicating if the player won, and a boolean value indicating if it's a new skin
// Post: Updates the UI elements based on the result and new skin status
void ResultWindow::setResult(bool isWin, bool isNewSkin) {
    this->isWin = isWin;

    // File path for the GIF
    QString gifFilePath;

    // Update UI elements based on win or lose
    if (isWin) {
        // Set the win label and GIF file path
        ui->result_label->setPixmap(QString(":/img/img/win_text.png"));

        // If win play gif_win.mp4
        gifFilePath = ":/video/video/gif_win.gif";

        // Win sound effect
        this->music->setMedia(QUrl("qrc:/sound/sound/sound_win.mp3"));
        this->music->play();

        // Show img backup
        ui->pict_win->show();
        ui->pict_lose->hide();
    }
    else {
         // Set the lose label and GIF file path
        ui->result_label->setPixmap(QString(":/img/img/lose_text.png"));

        // If win play gif_lose.mp4
        gifFilePath = ":/video/video/gif_lose.gif";

        // Lose effect
        this->music->setMedia(QUrl("qrc:/sound/sound/sound_lose.wav"));
        this->music->play();

        // Show img backup
        ui->pict_win->hide();
        ui->pict_lose->show();
    }

    // Show or hide the medal and skin label based on new skin status
    if (isNewSkin) {
        // Show the medal and skin label if it's a new skin
        ui->medal->show();
        ui->skin_label->show();
    }
    else {
        // Hide the medal and skin label if it's not a new skin
        ui->medal->hide();
        ui->skin_label->hide();
    }

     // Set up the GIF animation
    QMovie* movie = new QMovie(gifFilePath);

    // Set the movie to the GIF label
    ui->gif_label->setMovie(movie);

    // Start the GIF animation
    movie->start();

    // Set the size policy of the QLabel to adjust the size of the widget
    ui->gif_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}
