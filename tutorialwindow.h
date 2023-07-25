#ifndef TUTORIALWINDOW_H
#define TUTORIALWINDOW_H

#include <QWidget>

namespace Ui {
class TutorialWindow;
}

class TutorialWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TutorialWindow(QWidget *parent = nullptr);
    ~TutorialWindow();

private slots:
    void on_back_btn_clicked();

signals:
    void BackToMenuClicked();

private:
    Ui::TutorialWindow *ui;
};

#endif // TUTORIALWINDOW_H
