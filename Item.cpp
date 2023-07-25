/***********************************************************************
 * Author:  B11115010 鍾詩靈
 *          B11115014 魏美芳
 * Create Date: 2023/05/24
 * Update Date: 2023/06/01
 * Description: This C++ program performs dungeon game with kirby theme that have many features (see readme file)
***********************************************************************/

#include "Item.h"
 
// The constructor sIcon to 'T
Item::Item() {
    sIcon = 'T';
    this->sIsTriggered = false;
}

// The constructor takes x and y as parameters, sets sPos.x to x, sPos.y to y, and sIcon to 'T'
Item::Item(int x, int y) {
    sPos.x = x;
    sPos.y = y;
    sIcon = 'T';
    this->sIsTriggered = false;
}

// The copy constructor creates a new Item object by copying the values from ref
Item::Item(const Item& ref) {
    *this = ref;
}

// Update template
void Item::update(Hero& hero) {
}

// Get isTriggered
bool Item::getHasTriggered() {
    return this->sIsTriggered;
}

// Set isTriggered 
void Item::setHasTriggered(bool isTriggered) {
    this->sIsTriggered = isTriggered;
}

// The setPos function sets the sPos member variable to pos
void Item::setPos(const Position& pos) {
    this->sPos = pos;
}

// The setPos function sets sPos.x to x and sPos.y to y
void Item::setPos(int x, int y) {
    this->sPos.x = x;
    this->sPos.y = y;
}

// The getPos function returns a copy of sPos
Position Item::getPos() const {
    return this->sPos;
}

// The getIcon function returns the value of sIcon
char Item::getIcon() const {
    return this->sIcon;
}

// Get the imgUrl
QString Item::getImgUrl() {
    // Construct the URL using the image URL
    QString url = ":/img/img/" + this->sImgUrl;

    // Return the constructed URL
    return url;
}

// Set the imgUrl
void Item::setImgUrl(QString url) {
    this->sImgUrl = url;
}

// Set icon
void Item::setIcon(char& icon)
{
    this->sIcon = icon;
}

// Get Icon
char Item::getIcon()
{
    return this->sIcon;
}
