#pragma once
#include <QString>
#include "Position.h"
#include "Hero.h"

class Hero; // Forward declaration of Hero class
 
class Item {
protected:
    Position sPos;
    char sIcon;
    QString sImgUrl;
    bool sIsTriggered;

public:
    Item();
    Item(int x, int y);
    Item(const Item& ref);
    virtual void update(Hero& hero);
    void setPos(const Position& pos);
    void setPos(int x, int y);
    Position getPos() const;
    char getIcon() const;
    QString getImgUrl();
    void setImgUrl(QString url);
    void setIcon(char& icon);
    char getIcon();
    bool getHasTriggered();
    void setHasTriggered(bool isTriggered);

};
