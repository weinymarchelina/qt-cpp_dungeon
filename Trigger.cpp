/***********************************************************************
 * Author:  B11115010 鍾詩靈
 *          B11115014 魏美芳
 * Create Date: 2023/05/24
 * Update Date: 2023/06/01
 * Description: This C++ program performs dungeon game with kirby theme that have many features (see readme file)
***********************************************************************/

#include "Trigger.h"

// The constructor initializes sExp to 10 and sIcon to 'T
Trigger::Trigger() : sExp(10) {
    sIcon = 'T';
    this->sIsTriggered = false;
    this->sImgUrl = "trigger.png";
}

// The constructor takes x and y as parameters, initializes sExp to 10, sets sPos.x to x, sPos.y to y, and sIcon to 'T'
Trigger::Trigger(int x, int y) : sExp(10) {
    sPos.x = x;
    sPos.y = y;
    sIcon = 'T';
    this->sIsTriggered = false;
    this->sImgUrl = "trigger.png";
}

// The copy constructor creates a new Trigger object by copying the values from ref
Trigger::Trigger(const Trigger& ref) {
    *this = ref;
}

// The update function checks if hero's position matches Trigger's position and if so, the hero gains experience equal to sExp
void Trigger::update(Hero& hero) {
    // If the positions match, the hero gains experience points equal to the value stored in the sExp member variable
    if (hero.getPos().x == sPos.x && hero.getPos().y == sPos.y) {

        // If the positions match, the hero gains experience points equal to the value stored in the sExp member variable
        hero.gainEXP(sExp);
        this->sIsTriggered = true;
    }
}

// The getExpAmount function returns the value of sExp
int Trigger::getExpAmount() const {
    return this->sExp;
}
