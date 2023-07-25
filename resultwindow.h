#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QMovie>
#include <QMediaPlayer>

namespace Ui {
class ResultWindow;
}

class ResultWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ResultWindow(QWidget *parent = nullptr);
    ~ResultWindow();

    void setResult(bool isWin, bool isNewSkin);

private slots:
    void on_back_btn_clicked();

signals:
    void BackToMenuClicked();

private:
    Ui::ResultWindow *ui;
    bool isWin = false;

    // Music player
    QMediaPlayer* music = new QMediaPlayer;
};

#endif // RESULTWINDOW_H
