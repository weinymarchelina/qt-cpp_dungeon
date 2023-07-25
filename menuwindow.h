#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QMainWindow>
#include "customwindow.h"
#include "gamewindow.h"
#include "resultwindow.h"
#include "tutorialwindow.h"
#include <QString>
#include <QVector>
#include <QKeyEvent>
#include <fstream>

QT_BEGIN_NAMESPACE
namespace Ui { class MenuWindow; }
QT_END_NAMESPACE

class MenuWindow : public QMainWindow
{
    Q_OBJECT

public:
    MenuWindow(QWidget *parent = nullptr);
    ~MenuWindow();

private slots:
    void moveToMenu();
    void moveToCustom();
    void moveToGame();
    void moveToResult();
    void moveToTutorial();

    void on_custom_btn_clicked();

    void on_play_btn_clicked();

    void on_custom_btn_2_clicked();

    void on_easy_btn_clicked();

    void on_medium_btn_clicked();

    void on_hard_btn_clicked();

    // Handle arrow
    void handleArrow();

    // Key Press Event
    void keyPressEvent(QKeyEvent *e);

    // Load records
    void loadRecords();


    void on_left_arrow_clicked();

    void on_right_arrow_clicked();

    void on_music_btn_clicked();

private:
    Ui::MenuWindow *ui;
    CustomWindow _CustomPage;
    GameWindow _GamePage;
    ResultWindow _ResultPage;
    TutorialWindow _TutorialPage;

    int currentSkinIndex = 5;
    QString currentSkinImg = ":/img/img/main_kirby_5.png";
    QMediaPlayer* music = new QMediaPlayer;
    char currentKey = '\0';
    int selectedDifficulty = 1;
    bool isMusicOn = true;
    QVector<int> easyProperties = {17, 3, 1, 1, 3, 5, 20, 17};
    QVector<int> mediumProperties = {15, 3, 2, 2, 3, 3, 10, 15};
    QVector<int> hardProperties = {12, 3, 3, 3, 2, 2, 5, 12};

    QString selectedStyle = "QPushButton { padding: 20px 0; text-transform: uppercase; color: #F8E9EC; background-color: #90013B; }";
    QString unselectedStyle = "QPushButton { padding: 20px 0; text-transform: uppercase; color: #90013B; background-color: #F8E9EC; } QPushButton:hover { padding: 20px 0; text-transform: uppercase; color: #F8E9EC; background-color: #90013B; } ";

    // Initialize records
    std::vector<bool> gameRecords;
};
#endif // MENUWINDOW_H
