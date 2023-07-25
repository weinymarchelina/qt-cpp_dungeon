#ifndef SQUARE_H
#define SQUARE_H

#include <QWidget>
#include <QLabel>
#include <QString>

class Square : public QLabel
{
    Q_OBJECT

public:
    Square(const QString& imageUrl, QWidget* parent = nullptr);

    void changeImg(const QString& url);
    void setNoImg();
    void setStretched(bool isStrecthed);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QString imageUrl;
    bool hasImage;
    bool isStretched = false;
};

#endif // SQUARE_H
