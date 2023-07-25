/***********************************************************************
 * Author:  B11115010 鍾詩靈
 *          B11115014 魏美芳
 * Create Date: 2023/05/24
 * Update Date: 2023/06/01
 * Description: This C++ program performs dungeon game with kirby theme that have many features (see readme file)
***********************************************************************/

#include "menuwindow.h"
#include "ui_menuwindow.h"

// Intent: Initialize the MenuWindow class and set up the user interface for the menu window of the game.
// Pre: A valid QWidget pointer is passed as the parent.
// Post: The MenuWindow object is created, and the user interface is set up with various widgets and connections.
MenuWindow::MenuWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MenuWindow)
{
    // Set up the user interface
    ui->setupUi(this);

    // Set the window title
    setWindowTitle("Kirby: Down in the Dungeon Game");

    // Insert the custom page, game page, result page, and tutorial page into the stacked widget
    ui->stackedWidget->insertWidget(1, &_CustomPage);
    ui->stackedWidget->insertWidget(2, &_GamePage);
    ui->stackedWidget->insertWidget(3, &_ResultPage);
    ui->stackedWidget->insertWidget(4, &_TutorialPage);

    // Connect signals and slots for various interactions within the menu window
    connect(&_CustomPage, SIGNAL(BackToMenuClicked()), this, SLOT(moveToMenu()));
    connect(&_CustomPage, SIGNAL(StartGameClicked()), this, SLOT(moveToGame()));
    connect(&_ResultPage, SIGNAL(BackToMenuClicked()), this, SLOT(moveToMenu()));
    connect(&_GamePage, SIGNAL(ShowResultClicked()), this, SLOT(moveToResult()));
    connect(&_TutorialPage, SIGNAL(BackToMenuClicked()), this, SLOT(moveToMenu()));

    // Set up the background music
    this->music->setMedia(QUrl("qrc:/sound/sound/music_5.mp3"));
    this->music->play();
    this->music->setVolume(30);

    // Connect to mediaStatusChanged signal
    connect(this->music, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status) {
        // Check whether the music has already finished being play
        if (status == QMediaPlayer::EndOfMedia) {
            // Restart the music when it has finished playing
            this->music->play();
        }
    });

    // Start playing the background music
    this->music->play();

    // Load records from a file or database
    loadRecords();

    // Handle the arrow functionality
    handleArrow();
}

// Intent: Destruct the MenuWindow object and free up memory.
// Pre: None.
// Post: The MenuWindow object is deleted.
MenuWindow::~MenuWindow()
{
    delete ui;
}

// Intent: Load the game records from a file or database.
// Pre: The game_data_records file exists.
// Post: The gameRecords vector is populated with the loaded records.
void MenuWindow::loadRecords()
{
    // Set the 5 records to false and last 1 to true (basic skin)
    for (int i = 0; i < 5; i++) {
        gameRecords.push_back(false);
    }

    // Set the last record to true (basic skin)
    gameRecords.push_back(true);

    // Set up ifstream for previous game data records
    std::ifstream recordsFile("game_data_records");

    // Read previous records if available; otherwise create new
    if (recordsFile.is_open()) {
        // Variable to store the record type
        int recordType = -1;

        // Read the data on the records
        while (recordsFile >> recordType) {
            // If the record is valid (0 - 4), then set it to true according to the index
            if (recordType >= 0 && recordType <= 4) {
                gameRecords[recordType] = true;
            }
        }

        // Close the file stream
        recordsFile.close();
    }
}

// Intent: Handle the visibility of the arrow buttons based on the availability of game records.
// Pre: The gameRecords vector is populated.
// Post: The arrow buttons are shown or hidden based on the availability of game records.
void MenuWindow::handleArrow() {
    bool recordsAvailable = false;

    // Check if any game record is available
    for (int i = 0; i < 5; i++) {
        if (gameRecords[i]) {
            recordsAvailable = true;
            break;
        }
    }

    // Show or hide the arrow buttons based on the availability of game records
    if (recordsAvailable) {
        ui->left_arrow->show();
        ui->right_arrow->show();
    }
    else {
        ui->left_arrow->hide();
        ui->right_arrow->hide();
    }
}

// Intent: Handle the key press event for the menu window.
// Pre: A valid QKeyEvent pointer is passed.
// Post: The currentKey variable is updated, and the pressed key is passed to the 
void MenuWindow::keyPressEvent(QKeyEvent *e) {
    // Update the currentKey variable with the pressed key
    this->currentKey = char(e->key());

    // Pass the pressed key to the _GamePage
    _GamePage.setPressedKey(this->currentKey);
}

// Intent: Move to the menu page.
// Pre: The necessary UI elements are initialized and set up properly.
// Post: The menu page is displayed, and necessary actions are performed.
void MenuWindow::moveToMenu() {
    // Play the music
    music->play();

    // Load the game records
    this->loadRecords();

    // Handle the visibility of arrow buttons
    this->handleArrow();

    // Click the easy button
    this->on_easy_btn_clicked();

    // Set the current page to the menu page
    ui->stackedWidget->setCurrentIndex(0);
};

// Intent: Move to the custom page.
// Pre: The necessary UI elements are initialized and set up properly.
// Post: The custom page is displayed, and necessary actions are performed.
void MenuWindow::moveToCustom() {
    // Set the selected difficulty to 0 (custom)
    this->selectedDifficulty = 0;

    // Reset the spin box in the custom page
    _CustomPage.resetSpinBox();

    // Set the current page to the custom page
    ui->stackedWidget->setCurrentIndex(1);
};

// Intent: Move to the game page.
// Pre: The selected difficulty is a valid value (1, 2, 3, or 0).
// Post: The game page is displayed, and necessary actions are performed.
void MenuWindow::moveToGame() {
    QVector<int> properties;

    // Determine the properties based on the selected difficulty
    if (this->selectedDifficulty == 1) {
        properties = easyProperties;
    }
    else if (this->selectedDifficulty == 2) {
        properties = mediumProperties;
    }
    else if (this->selectedDifficulty == 3) {
        properties = hardProperties;
    }
    else if (this->selectedDifficulty == 0) {
        properties = _CustomPage.customProperties;
    }

    // Handle the difficulty properties in the game page
    _GamePage.handleDifficultyProperties(properties);

    // Set the hero skin in the game page
    _GamePage.setHeroSkin(this->currentSkinImg);

    // Set the current page to the game page
    ui->stackedWidget->setCurrentIndex(2);
};

// Intent: Move to the result page.
// Pre: The necessary game records have been updated.
// Post: The result page is displayed, and necessary actions are performed.
void MenuWindow::moveToResult() {
    bool isNewSkin = false;

    // Setup records for being not damaged (not available in custom mode)
    if (_GamePage.getHasNotDamaged() && this->selectedDifficulty != 0) {
        // Check for new skin
        if (!gameRecords[4]) {
            isNewSkin = true;
        }

        // Update the record for not being damaged
        gameRecords[4] = true;
    }

    // Get game result
    bool isWin = _GamePage.getIsWin();

    // Setup records for completing the level
    if (isWin && this->selectedDifficulty != 0) {
        // Check for new skin
        if (!gameRecords[this->selectedDifficulty]) {
            isNewSkin = true;
        }

        // Update the record for completing the level
        gameRecords[this->selectedDifficulty] = true;
    }

    // Setup records for completing all level
    if (gameRecords[1] && gameRecords[2] && gameRecords[3]) {
        // Check for new skin
        if (!gameRecords[this->selectedDifficulty]) {
            isNewSkin = true;
        }

        // Update the record for completing all levels
        gameRecords[0] = true;
    }

    // Write new records
    std::ofstream newRecordsFile("game_data_records");

    // Save the records on game_data_records
    for (int i = 0; i < 5; i++) {

        // Check if the game record at index i exists
        if (gameRecords[i]) {
            newRecordsFile << i << std::endl;
        }
    }

    // Send result to result page
    _ResultPage.setResult(isWin, isNewSkin);

    // Pause music
    this->music->pause();

    // Move to result page
    ui->stackedWidget->setCurrentIndex(3);
};

// Intent: Move to the tutorial page
// Pre: None
// Post: The current index of the stacked widget is set to 4
void MenuWindow::moveToTutorial() {
    ui->stackedWidget->setCurrentIndex(4);
};

// Intent: Handle the custom button click event
// Pre: None
// Post: Calls the moveToCustom function
void MenuWindow::on_custom_btn_clicked()
{
    this->moveToCustom();
}

// Intent: Handle the play button click event
// Pre: None
// Post: Calls the moveToGame function
void MenuWindow::on_play_btn_clicked()
{
    this->moveToGame();
}

// Intent: Handle the second custom button click event
// Pre: None
// Post: Calls the moveToTutorial function
void MenuWindow::on_custom_btn_2_clicked()
{
    this->moveToTutorial();
}

// Intent: Handle the easy button click event
// Pre: None
// Post: Sets the selectedDifficulty to 1, updates the button styles accordingly
void MenuWindow::on_easy_btn_clicked()
{
    this->selectedDifficulty = 1;
    ui->easy_btn->setStyleSheet(this->selectedStyle);
    ui->medium_btn->setStyleSheet(this->unselectedStyle);
    ui->hard_btn->setStyleSheet(this->unselectedStyle);
}

// Intent: Handle the medium button click event
// Pre: None
// Post: Sets the selectedDifficulty to 2, updates the button styles accordingly
void MenuWindow::on_medium_btn_clicked()
{
    this->selectedDifficulty = 2;
    ui->easy_btn->setStyleSheet(this->unselectedStyle);
    ui->medium_btn->setStyleSheet(this->selectedStyle);
    ui->hard_btn->setStyleSheet(this->unselectedStyle);
}

// Intent: Handle the hard button click event
// Pre: None
// Post: Sets the selectedDifficulty to 3, updates the button styles accordingly
void MenuWindow::on_hard_btn_clicked()
{
    this->selectedDifficulty = 3;
    ui->easy_btn->setStyleSheet(this->unselectedStyle);
    ui->medium_btn->setStyleSheet(this->unselectedStyle);
    ui->hard_btn->setStyleSheet(this->selectedStyle);
}

// Intent: Handle the left arrow button click event
// Pre: None
// Post: Decreases the currentSkinIndex, updates the current skin image and music 
void MenuWindow::on_left_arrow_clicked()
{
    this->currentSkinIndex -= 1;

    // Wrap around to the last index if currentSkinIndex becomes less than 0
    while (true) {
        // Wrap around to the last index if currentSkinIndex becomes less than 0
        if (this->currentSkinIndex < 0) {
            this->currentSkinIndex = 5;
        }

        // Break the loop if a game record exists for the current skin index
        if (gameRecords[this->currentSkinIndex]) {
            break;
        }

        // Decrement the currentSkinIndex
        this->currentSkinIndex--;
    }


    // Update the current skin image
    QString imgUrl = ":/img/img/main_kirby_" + QString::number(this->currentSkinIndex) + ".png";
    this->currentSkinImg = imgUrl;
    ui->img_kirby->setPixmap(imgUrl);

    // Update the current music
    QString musicUrl = "qrc:/sound/sound/music_" + QString::number(this->currentSkinIndex) + ".mp3";
    this->music->setMedia(QUrl(musicUrl));
    this->music->play();
}

// Intent: Handle the right arrow button click event
// Pre: None
// Post: Increases the currentSkinIndex, updates the current skin image and music based on the new index
void MenuWindow::on_right_arrow_clicked()
{
    this->currentSkinIndex += 1;

    // Wrap around to the first index if currentSkinIndex becomes greater than 5
    while (true) {
        // Wrap around to the first index if currentSkinIndex becomes greater than 5
        if (this->currentSkinIndex > 5) {
            this->currentSkinIndex = 0;
        }

        // Break the loop if a game record exists for the current skin index
        if (gameRecords[this->currentSkinIndex]) {
            break;
        }

        // Increment the currentSkinIndex
        this->currentSkinIndex++;
    }

    // Update the current skin image
    QString imgUrl = ":/img/img/main_kirby_" + QString::number(this->currentSkinIndex) + ".png";
    this->currentSkinImg = imgUrl;
    ui->img_kirby->setPixmap(imgUrl);

    // Update the current music
    QString musicUrl = "qrc:/sound/sound/music_" + QString::number(this->currentSkinIndex) + ".mp3";
    this->music->setMedia(QUrl(musicUrl));
    this->music->play();
}

// Intent: Handle the music button click event
// Pre: None
// Post: Toggles the music on/off based on the current state
void MenuWindow::on_music_btn_clicked()
{
    // Toggle the isMusicOn flag
    this->isMusicOn = !isMusicOn;

    // Play or pause the music based on the current state
    if (isMusicOn) {
        music->play();
    }
    else {
        music->pause();
    }
}

