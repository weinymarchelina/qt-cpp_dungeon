#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "Position.h"
#include "Hero.h"
#include "Creature.h"
#include "Scarfy.h"
#include "Bomber.h"
#include "Item.h"
#include "Trigger.h"
#include "Food.h"
#include "square.h"

#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <fstream>

#include <QWidget>
#include <QVector>
#include <QString>
#include <QPushButton>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QPainter>
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
#include <QMediaPlayer>


namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);

    ~GameWindow();

    // Handle difficulty difference
    void handleDifficultyProperties(QVector<int> properties);

    // Initialize new game
    void initializeNewGame();

    // Update key press
    void keyUpdate(bool key[]);

    // Create new icon board
    void initializeNewBoard();

    // Draw the icon board
    void drawBoard(void);

    // Create squares and setup board on UI
    void setupUIBoard(QVector<QVector<char>> drawBoard);

    // Print information
    void drawInfo(void);

    // Update the game state after each key press
    void update(bool key[]);

    // Save game function
    void saveMap();

    // Load game function
    void loadMap();

    // Check whether position valid
    bool checkPositionValid(Position pos);

    // Get random valid position
    Position getRandomPosition();

    // Get all filled position
    QVector<Position> getAllFilledPos();

    // Set pressed key
    void setPressedKey(char key);

    // Handle new key
    void handlePressedKey();

    // Handle game over
    void handleGameOver();

    // Get isWin
    bool getIsWin();

    // Get hasNotDamaged
    bool getHasNotDamaged();

    // Show error message
    void printError(QString msg);

    // Set Hero Skin
    void setHeroSkin(QString skin);


private slots:
    void on_save_btn_clicked();

    void on_load_btn_clicked();

signals:
    void ShowResultClicked();

private:
    Ui::GameWindow *ui;

    // Board square elements
    QVector<QVector<Square*>> squaresUI;

    // Initialize hero
    Hero gHero;

    // Hero skin
    QString heroSkin;

    // Difficulty Level
    int difficulty;

    // Invisible record
    bool hasNotDamaged = true;

    // Constant board value
    const char GWALL = 'O';
    const char GNOTHING = ' ';

    // Initialize game board
    QVector<QVector<char>> gBoard;

    // Initialize board size
    int gWidth = -1;
    int gHeight = -1;

    // Initialize vectors to store game's objects
    QVector<Creature*> gCreatures;
    QVector<Item*> gItems;

    // Intialize different enemy type counts and item type counts
    int deeCount = 0;
    int scarfyCount = 0;
    int bomberCount = 0;
    int triggerCount = 0;
    int foodCount = 0;
    int tomatoDropRate = 0;

    // Initialize gameOver and isWin
    bool isGameOver = false;
    bool isWin = false;

    // Store all saved file name during the game
    QVector<QString> savedFileName;

    // Initialize records
    QVector<bool> gameRecords;

    // Current key
    char currentKey = '\0';

    // Timer
    QTimer* timer = new QTimer(this);

    // Initalize enum for input key
    enum VALID_INPUT
    {
        EW = 0,
        ES = 1,
        EA = 2,
        ED = 3,
        ESPACE = 4,
        EI = 5,
        EL = 6,
        EK = 7,
        EJ = 8,
        EP = 9,
        EESC = 10,
        INVALID,
    };

    // Music player
    QMediaPlayer* music = new QMediaPlayer;
};

#endif // GAMEWINDOW_H
