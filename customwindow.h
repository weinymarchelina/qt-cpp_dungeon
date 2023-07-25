#ifndef CUSTOMWINDOW_H
#define CUSTOMWINDOW_H

#include <QWidget>
#include <QVector>

namespace Ui {
class CustomWindow;
}

class CustomWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CustomWindow(QWidget *parent = nullptr);
    ~CustomWindow();
    QVector<int> customProperties;
    void resetSpinBox();


private slots:
    void on_back_btn_clicked();

    void on_play_btn_clicked();

signals:
    void BackToMenuClicked();
    void StartGameClicked();

private:
    Ui::CustomWindow *ui;
};

#endif // CUSTOMWINDOW_H
