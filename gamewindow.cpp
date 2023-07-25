/***********************************************************************
 * Author:  B11115010 鍾詩靈
 *          B11115014 魏美芳
 * Create Date: 2023/05/24
 * Update Date: 2023/06/01
 * Description: This C++ program performs dungeon game with kirby theme that have many features (see readme file)
***********************************************************************/

#include "gamewindow.h"
#include "ui_gamewindow.h"

// Intent: Construct the GameWindow object
// Pre: Parent widget must be valid
// Post: GameWindow object is created and initialized
GameWindow::GameWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWindow)
{
    // Set up the UI
    ui->setupUi(this);

    // Seed the random number generator
    srand(time(NULL));

    // Connect the timer's timeout signal to hide error bubble and text
    connect(timer, &QTimer::timeout, this, [=]() {
        ui->error_bubble->hide();
        ui->error_text->hide();
    });

    // Connect the reset_focus button's clicked signal to clear focus from the filename_edit
    connect(ui->reset_focus, &QPushButton::clicked, this, [=]() {
        ui->filename_edit->clearFocus();
    });

    // Set the focus policy of the filename_edit to NoFocus
    ui->filename_edit->setFocusPolicy(Qt::NoFocus);
}

// Print error
void GameWindow::printError(QString msg) {
    // Set the error message text
    ui->error_text->setText(msg);

    // Show the error bubble and text
    ui->error_bubble->show();
    ui->error_text->show();

    // Timer for bubble text for 2s
    timer->start(2000);
}

// Intent: Destruct the GameWindow object
// Pre: None
// Post: GameWindow object is destroyed and memory is freed
GameWindow::~GameWindow()
{
    delete ui;
}

// Intent: Get all filled positions in the game
// Pre: None
// Post: Filled positions are returned as a vector of Position objects
QVector<Position> GameWindow::getAllFilledPos() {
    // Create a vector to store the filled positions
    QVector<Position> filledPositions;

    // Add the hero's position to the vector
    filledPositions.push_back(gHero.getPos());

    // Add the positions of all creatures to the vector
    for (auto& creature : gCreatures) {
        filledPositions.push_back(creature->getPos());
    }

    // Add the positions of all items to the vector
    for (auto& item : gItems) {
        filledPositions.push_back(item->getPos());
    }

    return filledPositions;
}

// Intent: Check if a position is valid (within the game boundaries and not occupied by other objects)
// Pre: Position pos must be valid
// Post: Returns true if the position is valid, false otherwise
bool GameWindow::checkPositionValid(Position pos) {
    // Check if the position is within the game boundaries
    if (!pos.isInRange(this->gWidth, this->gHeight)) {
        return false;
    }

    // Get all filled positions in the game
    QVector<Position> filledPositions = getAllFilledPos();

    // Check if the position is occupied by any filled positions
    for (auto& filledPos : filledPositions) {
        // Check if the position matches any of the filled positions
        if (pos == filledPos) {
            return false;
        }
    }

    return true;
}

// Intent: Get a random valid position within the game boundaries
// Pre: None
// Post: Returns a random valid Position object
Position GameWindow::getRandomPosition() {
    // Get random new position
    Position newPos = Position((int)(rand() % this->gWidth), (int)(rand() % this->gHeight));

    // Keep getting new position if the position is not valid
    while (!checkPositionValid(newPos)) {
        newPos = Position((int)(rand() % this->gWidth), (int)(rand() % this->gHeight));
    }

    return newPos;
}

// Intent: Set the hero's skin image
// Pre: The provided skin must be a valid image URL
// Post: The hero's skin is set to the provided image URL
void GameWindow::setHeroSkin(QString skin) {
    // Set the hero's skin image URL
    this->heroSkin = skin;

    // Set the pixmap of the info_img QLabel to the hero's skin image
    ui->info_img->setPixmap(this->heroSkin);
}

// Intent: Handle the difficulty properties provided for the game
// Pre: The properties vector must contain the required values in the correct order
// Post: The game properties are set based on the provided values, and a new game is initialized
void GameWindow::handleDifficultyProperties(QVector<int> properties) {
    // Set the game properties based on the provided values
    this->gWidth = properties[0];
    this->gHeight = properties[7];
    this->deeCount = properties[1];
    this->scarfyCount = properties[2];
    this->bomberCount = properties[3];
    this->triggerCount = properties[4];
    this->foodCount = properties[5];
    this->tomatoDropRate = properties[6];

    // Initialize a new game
    this->initializeNewGame();
}

// Intent: Initialize a new game
// Pre: None
// Post: A new game is initialized with the current game properties, and the UI is updated
void GameWindow::initializeNewGame() {
    // Clear the layout and squaresUI vectors
    QGridLayout* layout = ui->grid_board;

    // Create new Square widgets and add them to the layout
    for (int i = 0; i < this->gHeight; ++i) {
        // Create a vector to store the Square pointers for each row
        QVector<Square*> row;

        // Create and add Square widgets to the layout for each column
        for (int j = 0; j < this->gWidth; ++j) {
             // Create a new Square widget
            Square* square = new Square("");

             // Add the Square pointer to the row vector
            row.push_back(square);

             // Add the Square widget to the layout at the specified position
            layout->addWidget(square, i, j);
        }

         // Add the row vector to the squaresUI vector
        squaresUI.push_back(row);
    }

    // Initialize a new board with the current game properties
    this->initializeNewBoard();

    // Draw the board and info on the UI
    this->drawBoard();
    this->drawInfo();

    // Hide the error bubble and text
    ui->error_bubble->hide();
    ui->error_text->hide();


    // Set the focus policy of the filename_edit to ClickFocus
    ui->filename_edit->setFocusPolicy(Qt::ClickFocus);
}

// Intent: Initialize a new game board
// Pre: None
// Post: The game board is initialized with walls, hero, creatures, and items
void GameWindow::initializeNewBoard() {

    // Create the board with walls
    for (int i = 0; i < this->gHeight; i++) {
        // Create a new row vector to store the characters representing each cell
        QVector<char> newRow;

        // Iterate over each column
        for (int j = 0; j < this->gWidth; j++) {
            // Check if the cell is on the boundary (wall)
            if (i == 0 || i == this->gHeight - 1 || j == 0 || j == this->gWidth - 1) {
                // Add a wall character to the row
                newRow.push_back(GWALL);

                // Change the image of the corresponding Square widget to wall
                squaresUI[i][j]->changeImg(":/img/img/tile_wall.png");

                // Set the stretched property of the Square widget to true
                squaresUI[i][j]->setStretched(true);
            }
            else {
                // Add a non-wall character to the row
                newRow.push_back(GNOTHING);
            }
        }

        // Add the row to the game board
        gBoard.push_back(newRow);
    }

    // Generates a random position (hPos) for the hero
    Position hPos = this->getRandomPosition();

    // Set the position of the hero
    gHero.setPos(hPos);

    // Creates instances of the Creature classes
    for (int i = 0; i < this->deeCount; i++) {
        Creature* waddleDee = new Creature();
        gCreatures.push_back(waddleDee);
    }

    // Creates instances of the Scarfy classes
    for (int i = 0; i < this->scarfyCount; i++) {
        Creature* scarfy = new Scarfy();
        gCreatures.push_back(scarfy);
    }

    // Creates instances of the Bomber classes
    for (int i = 0; i < this->bomberCount; i++) {
        Creature* bomber = new Bomber();
        gCreatures.push_back(bomber);
    }

    // It generates valid random positions for each creature in gCreatures
    for (auto& creature : gCreatures) {
        Position cPos = this->getRandomPosition();
        creature->setPos(cPos);
        creature->setWidthHeight(this->gWidth, this->gHeight);
    }

    // It creates two instances of the Trigger class, and put them into the gItems vector
    for (int i = 0; i < this->triggerCount; i++) {
        Trigger* trigger = new Trigger();
        gItems.push_back(trigger);
    }

    // It creates five instances of the Food class, and put them into the gItems vector
    for (int i = 0; i < this->foodCount; i++) {
        Food* food = new Food(tomatoDropRate);
        gItems.push_back(food);
    }

    // It generates valid random positions for each item in gItems
    for (auto& item : gItems) {
        Position iPos = this->getRandomPosition();
        item->setPos(iPos);
    }
}

// Intent: Draw the game board on the UI
// Pre: The game board and objects (hero, creatures, items) are set
// Post: The game board is displayed on the UI
void GameWindow::drawBoard() {
    // Add the hero into the board
    QVector<QVector<char>> drawBoard(this->gHeight);

    // Setup and implementation of drawing board using 2d vector
    for (int i = 0; i < this->gHeight; i++) {
        // Resize each row to match the width of the game board
        drawBoard[i].resize(this->gWidth);

        // Copy the characters from the game board to the temporary board
        for (int j = 0; j < this->gWidth; j++) {
            drawBoard[i][j] = gBoard[i][j];
        }
    }

    // Draw the items using for loop on drawBoard
    for (int i = 0; i < gItems.size(); i++) {
        Position t = gItems[i]->getPos();
        drawBoard[t.y][t.x] = gItems[i]->getIcon();
    }

    // Draw the creatures using for loop on drawBoard
    for (int i = 0; i < gCreatures.size(); i++) {
        Position c = gCreatures[i]->getPos();
        drawBoard[c.y][c.x] = gCreatures[i]->getIcon();
    }

    // Update the hero information
    Position h = gHero.getPos();
    drawBoard[h.y][h.x] = gHero.getIcon();

    // Send the drawn board to UI setup
    this->setupUIBoard(drawBoard);

}

// Intent: Set up the UI board based on the drawn board
// Pre: The drawn board is provided as a parameter
// Post: The UI board is set up with the appropriate images and styles
void GameWindow::setupUIBoard(QVector<QVector<char>> drawBoard) {
    // Iterate over the rows of the drawn board
    for (int i = 0; i < this->gHeight; ++i) {
        // Iterate over the columns of the drawn board
        for (int j = 0; j < this->gWidth; ++j) {
            // Check if the current cell is not a wall
            if (drawBoard[i][j] != this->GWALL) {
                // Set the image of the UI square to empty
                squaresUI[i][j]->setNoImg();

                // Set the style sheet of the UI square
                squaresUI[i][j]->setStyleSheet("border: 1px solid #aaaaaa;");
            }
        }
    }

    // Iterate over the creatures in the game
    for (auto& creature : gCreatures) {
         // Get the position of the creature
        Position pos = creature->getPos();

        // Change the image of the UI square at the creature's position
        squaresUI[pos.y][pos.x]->changeImg(creature->getImgUrl());

        // Set condition to find Bomber type of creature
        bool isBomber = creature->getIcon() == 'B';

        // // Draw the bomber damage radius range on drawBoard when it has spotted the hero: Check whether bomber is still alive and seen the hero
        if (isBomber && creature->getHasSeen() && creature->getHealth()) {
            // Get the position of the active bomber
            Position b = creature->getPos();

            // Set radius to 1
            int radius = 1;

            // Draw the damage radius around the bomber on drawBoard
            for (int i = b.y - radius; i <= b.y + radius; i++) {
                // Iterate over the rows within the damage radius range
                for (int j = b.x - radius; j <= b.x + radius; j++) {
                    // Create a test position
                    Position testPos = Position(j, i);

                    // Check if the test position is within the bounds of the UI board
                    if (testPos.isInRange(this->gWidth, this->gHeight)) {
                        squaresUI[i][j]->setStyleSheet("background: red; border: 1px solid #202020;");
                    }
                }
            }

        }
    }

    // Change the image of the UI square at the hero's position
    squaresUI[gHero.getPos().y][gHero.getPos().x]->changeImg(gHero.getImgUrl());

    // Get the eat range of the hero
    QVector<Position> testCells = gHero.getEatRange();

    // Iterate over the test cells
    for (Position& cell : testCells) {
        // Check if the test cell is within the bounds of the UI board
        if (cell.isInRange(this->gWidth, this->gHeight)) {
            squaresUI[cell.y][cell.x]->setStyleSheet("background: pink; border: 1px solid #aaaaaa;");
        }
    }

    // Iterate over the items in the game
    for (auto& item : gItems) {
        // Get the position of the item
        Position pos = item->getPos();

        // Change the image of the UI square at the item's position
        squaresUI[pos.y][pos.x]->changeImg(item->getImgUrl());
    }
}

// Intent: Update the information displayed on the UI
// Pre: None
// Post: The information on the UI is updated with the latest values
void GameWindow::drawInfo() {
    // Update the attack, HP, and level display with the values from the hero
    ui->display_atk->setText(QString::number(gHero.getAttack()));
    ui->display_hp->setText(QString::number(gHero.getHP()));
    ui->display_level->setText(QString::number(gHero.getLevel()));

    // Calculate the percentage of experience progress
    double percentage = ((double)gHero.getExp() / (double)gHero.getMaxExp()) * 100;

    // Update the progress bar with the calculated percentage
    ui->progressBar->setValue((int)percentage);
}

// Intent: Update the state of the board
// Pre: The user key input as the parameter
// Post: The board is updated with the newest change of Hero and Creature based on the user key input
void GameWindow::update(bool key[])
{
    // Declares a delta variable of type Position to represent the change in position
    Position delta;

    // A boolean variable hasInput is initialized as false to keep track of whether any valid input has been received
    bool hasInput = false;

    // The function checks the values in the key array to determine the input and also set direction of the hero
    if (key[VALID_INPUT::EW]) {
        delta -= Position(0, 1);
        hasInput = true;
        gHero.setDirection(0);
    }
    else if (key[VALID_INPUT::ES]) {
        delta += Position(0, 1);
        hasInput = true;
        gHero.setDirection(2);
    }
    else if (key[VALID_INPUT::EA]) {
        delta = delta - Position(1, 0);
        hasInput = true;
        gHero.setDirection(3);
    }
    else if (key[VALID_INPUT::ED]) {
        delta = delta + Position(1, 0);
        hasInput = true;
        gHero.setDirection(1);
    }
    else if (key[VALID_INPUT::EI]) {
        gHero.setDirection(0);
    }
    else if (key[VALID_INPUT::EK]) {
        gHero.setDirection(2);
    }
    else if (key[VALID_INPUT::EJ]) {
        gHero.setDirection(3);
    }
    else if (key[VALID_INPUT::EL]) {
        gHero.setDirection(1);
    }
    else if (key[VALID_INPUT::EP]) {
        // Call swallow function
        gHero.swallow(this->gWidth, this->gHeight, gCreatures, gItems, 10);
    }
    else {
        // Prints "Invalid input" to indicate that no valid input was detected
        this->printError("Invalid input~");
    }

    // If valid input is detected, calls the move the hero
    if (hasInput) {
        // Calculate the next position based on the current position and the delta
        Position next = gHero.getPos() + delta;
        bool triggerItem = false;

        // Check if the next position contains an item
        for (auto& item : gItems) {
             // Check if the item is a trigger item and its position matches the next position
            if (item->getIcon() == 'T' && item->getPos() == next) {
                // Move the hero to the next position
                gHero.setPos(next);
                triggerItem = true;
                break;
            }
        }

         // If no item was triggered, check if the next position is valid
        if (!triggerItem) {
            // Check if the next position is valid
            if (this->checkPositionValid(next))
                gHero.setPos(next);
            else {
                // Print error message when the hero can't move to the next position
                this->printError("I can't go there~");

                // Blocked sound effect
                this->music->setMedia(QUrl("qrc:/sound/sound/blocked_" + QString::number(rand() % 2) + ".wav"));
                this->music->play();
            }
        }
    }

    // Manipulate update of the items using while loop
    for (int i = 0; i < gItems.size(); i++) {
        gItems[i]->update(gHero);
    }

    // Setup deletedIndex vector as index marker
    QVector<int> deletedIndex;

    // Handle items different isTriggered state
    for (int i = gItems.size() - 1; i >= 0; i--) {
        // Handle hasTriggered cases
        if (gItems[i]->getHasTriggered()) {
            // For trigger, set new random location (Trigger moves when trigerred); for food, food gone when eaten (only if the hero's hp is not max)
            if (gItems[i]->getIcon() == 'T') {
                //	Set a random valid position for the new position of the trigger
                Position newPos = this->getRandomPosition();
                gItems[i]->setPos(newPos);
                gItems[i]->setHasTriggered(false);
            }
            else {
                // Eating sound effect
                this->music->setMedia(QUrl("qrc:/sound/sound/nyum.mp3"));
                this->music->play();

                // Add the deleted food to the vector
                deletedIndex.push_back(i);
            }
        }
    }

    // Delete eaten food
    for (int& index : deletedIndex) {
        // Remove the object from memory
        delete gItems[index];

        // Remove the food from the gItems vector
        gItems.erase(gItems.begin() + index);
    }

    //	Clear the deletedIndex to be used on gCreatures
    deletedIndex.clear();

    // Manipulate update of the creatures using while loop
    for (int i = 0; i < gCreatures.size(); i++) {
        gCreatures[i]->setFilledPositions(this->getAllFilledPos());
        gCreatures[i]->update(gHero);
    }

    // Find creature with hp = 0
    for (int i = gCreatures.size() - 1; i >= 0; i--) {
        // Push the index of dead creature to the vector
        if (!gCreatures[i]->getHealth()) {
            deletedIndex.push_back(i);
        }
    }

    // Delete dead creature
    for (int& index : deletedIndex) {
        // Remove the object from memory
        delete gCreatures[index];

        // Remove the creature from the gCreatures vector
        gCreatures.erase(gCreatures.begin() + index);
    }

    // Draw map and print information
    drawBoard();
    drawInfo();

    // Check whether the hero has ever been hurted
    if (gHero.getHP() < 100) {
        hasNotDamaged = false;
    }

    // Check current game condition
    if (gHero.getHP() <= 0) {
        // If the ehro health is less or equal to 0, then hero lose
        isGameOver = true;
        isWin = false;
    }
    else if (gCreatures.empty()) {
        // If there are no enemy left, then the hero win the game
        isGameOver = true;
        isWin = true;
    }
}

// Intent: Set the pressed key and handle it
// Pre: The key parameter indicates the key that was pressed
// Post: The currentKey variable is updated and the pressed key is handled
void GameWindow::setPressedKey(char key) {
    // Update the currentKey variable with the pressed key
    this->currentKey = key;

    // Handle the pressed key
    this->handlePressedKey();
}

// Intent: Handle the pressed key
// Pre: None
// Post: The game state is updated based on the pressed key
void GameWindow::handlePressedKey() {
    // Initialize map of key and whether the key is pressed or not
    bool gKeyState[VALID_INPUT::INVALID];

    // Update the key state based on the current key
    this->keyUpdate(gKeyState);

    // Update the game state based on the key state
    this->update(gKeyState);

    // Check if the game is over
    if (isGameOver) {
        // Handle the game over situation
       handleGameOver();

        // Emit a signal to show the game result
       emit ShowResultClicked();
    }
}

// Intent: Accept user key input and determine the pressed character based on the input
// Pre: Key parameter must be an array of booleans, representing key state
// Post: Set the pressed key value to true and the others to false
void GameWindow::keyUpdate(bool key[]) {
    // Initializes all elements of the key array to false
    for (int i = 0; i < VALID_INPUT::INVALID; i++) {
        key[i] = false;
    }

    // Get the pressed key
    char input = this->currentKey;
    this->currentKey = '\0';

    // Check the value of the input character
    switch (input) {
    case 'W':
        key[VALID_INPUT::EW] = true;
        break;
    case 'S':
        key[VALID_INPUT::ES] = true;
        break;
    case 'A':
        key[VALID_INPUT::EA] = true;
        break;
    case 'D':
        key[VALID_INPUT::ED] = true;
        break;
    case ' ':
        key[VALID_INPUT::ESPACE] = true;
        break;
    case 'I':
        key[VALID_INPUT::EI] = true;
        break;
    case 'L':
        key[VALID_INPUT::EL] = true;
        break;
    case 'K':
        key[VALID_INPUT::EK] = true;
        break;
    case 'J':
        key[VALID_INPUT::EJ] = true;
        break;
    case 'P':
        key[VALID_INPUT::EP] = true;
        break;
    case 27:
        key[VALID_INPUT::EESC] = true;
        break;
    default:
        break;
    }
}

// Intent: Handle the game over situation
// Pre: None
// Post: The game state is reset and the UI is cleared
void GameWindow::handleGameOver() {
    // Delete the file that was created when saving the game
    for (QString& filename : savedFileName) {
        std::string deletedFile = (filename + ".txt").toStdString();
        std::remove(deletedFile.c_str());
    }

    // Reset UI Board
    QGridLayout* layout = ui->grid_board;
    QLayoutItem* item;

    // Remove all widgets from the UI board layout and delete them
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Clear board template
    gBoard.clear();

    // Reset square references
    for (int i = 0; i < this->gHeight; ++i) {
        for (int j = 0; j < this->gWidth; ++j) {
            squaresUI[i][j] = nullptr;
        }
    }

    // Clear the squaresUI vector
    squaresUI.clear();


    // Reset the game properties
    Hero newHero;
    gHero = newHero;

    // Delete existing items and clear the gCreatures vectors
    for (int i = 0; i < gCreatures.size(); i++)
        delete gCreatures[i];
    gCreatures.clear();

    // Delete existing items and clear the gItems vector
    for (int i = 0; i < gItems.size(); i++)
        delete gItems[i];
    gItems.clear();

    // Reset gameOver state
    this->isGameOver = false;
}

// Intent: Get the win status of the game
// Pre: None
// Post: Returns true if the game is won, false otherwise
bool GameWindow::getIsWin() {
    return this->isWin;
}

// Intent: Get the status of whether the hero has been damaged or not
// Pre: None
// Post: Returns true if the hero has not been damaged, false otherwise
bool GameWindow::getHasNotDamaged() {
    return this->hasNotDamaged;
}


// Intent: Save the game map to a file
// Pre: None
// Post: The game map is saved to a file with the specified name
void GameWindow::saveMap() {
    // Trim to make sure the input is not empty spaces
    QString filename = ui->filename_edit->toPlainText().trimmed();

    // Turn QString to std string
    std::string input = filename.toStdString();

    // Prevents the user to name game_data_records or empty as file name to prevent overlaps
    if (input == "game_data_records" || input.empty()) {
        this->printError("Please input another file name~");
        return;
    }

    // Save the file name by storing it to vector
    savedFileName.push_back(filename);

    // It creates an output file stream oStream with the specified file name concatenated with ".txt"
    std::ofstream oStream(input + ".txt");

    // Writes the hero's position, HP, level, experience, and maximum experience, dir to the file
    oStream << gHero.getPos() << " " << gHero.getHP() << " " << gHero.getLevel() << " " << gHero.getExp() << " " << gHero.getMaxExp() << " " << gHero.getDirection() << "\n";

    // Writes the number of creatures in the gCreatures vector to the file
    oStream << gCreatures.size() << "\n";

    // For each creature in the vector
    for (int i = 0; i < gCreatures.size(); i++) {
        // Retrieves the current creature's icon using gCreatures[i]->getIcon()
        char currentIcon = gCreatures[i]->getIcon();

        // Based on the icon value, it determines the appropriate identifier, and writes it to the file
        if (currentIcon == 'C') {
            oStream << "C ";
        }
        else if (currentIcon == 'S') {
            oStream << "S ";
        }
        else if (currentIcon == 'B') {
            oStream << "B ";
        }

        // Writes the creature's position (getPos()) and health (getHealth()), direction, hasSeen to the file
        oStream << gCreatures[i]->getPos() << " " << gCreatures[i]->getHealth() << " " << gCreatures[i]->getDirection() << " " << gCreatures[i]->getHasSeen() << "\n";
    }

    // Writes the number of items in the gItems vector to the file
    oStream << gItems.size() << "\n";

    // Writes the number of items in the gItems vector to the file
    for (int i = 0; i < gItems.size(); i++) {
        // Retrieves the current item's icon using gItems[i]->getIcon()
        char currentIcon = gItems[i]->getIcon();

        // Retrieves the current item's position using gItems[i]->getPos()
        Position itemPos = gItems[i]->getPos();

        // Writes the item icon to know the item type
        oStream << currentIcon << " ";

        // Writes the creature's position (getPos()) and health (getHealth()) to the file
        oStream << itemPos << " " << "\n";
    }

    // The output file stream is closed
    oStream.close();
};

// Intent: Load a game map from a file
// Pre: None
// Post: The game map is loaded from the specified file
void GameWindow::loadMap() {
    // Get the filename from the input field
    QString filename = ui->filename_edit->toPlainText();

    // Convert the QString to std::string
    std::string input = filename.toStdString();

    // Open the input file
    std::ifstream iStream(input + ".txt");

    if (!iStream.is_open()) {
        this->printError("File is not found~");
        return;
    }

    qDebug() << "Opening: " << filename;

    // Delete existing items and clear the gCreatures vectors
    for (int i = 0; i < gCreatures.size(); i++)
        delete gCreatures[i];
    gCreatures.clear();

    // Delete existing items and clear the gItems vector
    for (int i = 0; i < gItems.size(); i++)
        delete gItems[i];
    gItems.clear();

    // Read the hero's position and attributes
    Position pos;
    int hp, level, exp, maxExp, dir;
    iStream >> pos >> hp >> level >> exp >> maxExp >> dir;

    // Sets Hero's position, HP, level, exp, maxExp
    gHero.setPos(pos);
    gHero.setHP(hp);
    gHero.setLevel(level);
    gHero.setExp(exp);
    gHero.SetMaxExp(maxExp);
    gHero.setDirection(dir);

    // Read the number of creatures
    int creatureN;
    iStream >> creatureN;

    // Read the creature data and create the creatures
    for (int i = 0; i < creatureN; i++) {
        char iconType = ' ';
        int hp = 1;
        int dir = 0;
        bool hasSeen = false;
        iStream >> iconType >> pos >> hp >> dir >> hasSeen;

        // A pointer to a Creature object named creature is initialized as nullptr
        Creature* creature = nullptr;

        // Based on the value of iconType, a new creature object is created and assigned to the creature pointer
        if (iconType == 'C') {
            creature = new Creature();
        }
        else if (iconType == 'S') {
            creature = new Scarfy();
        }
        else if (iconType == 'B') {
            creature = new Bomber();
        }

        // Sets the creature position and Hp and dir and hasSeen
        creature->setPos(pos);
        creature->setHealth(hp);
        creature->setDirection(dir);
        creature->setHasSeen(hasSeen);

        // Creates creature object is added to the gCreatures vector
        gCreatures.push_back(creature);
        creature->setWidthHeight(this->gWidth, this->gHeight);
    }

    // Read the number of items
    int itemN;
    iStream >> itemN;

    // Read the item data and create the items
    for (int i = 0; i < itemN; i++) {
        char iconType = ' ';
        iStream >> iconType >> pos;

        // A pointer to a Item object named item is initialized as nullptr
        Item* item = nullptr;

        // Based on the value of iconType, a new item object is created and assigned to the item pointer
        if (iconType == 'T') {
            item = new Trigger();
        }
        else {
            item = new Food(tomatoDropRate);
            item->setIcon(iconType);

            // qDebug() << "Food Icon: " << iconType;

            QString newUrl = "";

            if (iconType == 'M') {
                newUrl = "M_food.PNG";
            }
            else {

                newUrl = QString(iconType) + "_food.png";
            }

            item->setImgUrl(newUrl);
        }

        // Sets the item position
        item->setPos(pos);

        // Creates items object is added to the gItems vector
        gItems.push_back(item);
    }

    // Close the input file
    iStream.close();

    qDebug() << "All data is loaded!";

    // Reload board
    this->drawBoard();
    this->drawInfo();
};

// Intent: Handle the click event of the save button
// Pre: None
// Post: The current game map is saved to a file
void GameWindow::on_save_btn_clicked()
{
    this->saveMap();
}

// Intent: Handle the click event of the load button
// Pre: None
// Post: The game map is loaded from a file
void GameWindow::on_load_btn_clicked()
{
    this->loadMap();
}

