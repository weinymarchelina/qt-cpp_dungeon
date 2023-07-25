/***********************************************************************
 * Author:  B11115010 鍾詩靈
 *          B11115014 魏美芳
 * Create Date: 2023/05/24
 * Update Date: 2023/06/01
 * Description: This C++ program performs dungeon game with kirby theme that have many features (see readme file)
***********************************************************************/

#include "Bomber.h"

//The default constructor of the Bomber class initializes the member variables
Bomber::Bomber() {
    this->sIcon = 'B';
    this->power = 10;
    this->viewDistance = 3;
    this->chaseHeroProbability = 85;
    this->awareness = true;
    this->hp = 5;
    this->sImgUrl = "_bomber";
}

//The function of the Bomber class takes a reference to a Hero object hero as a parameter
void Bomber::updateDamage(Hero& hero) {
    //If the hp of the bomber is 0 (already dead), it returns early
    if (!this->hp) {
        return;
    }

    // If the bomber has not seen the hero, it resets its hp to 5 and returns early
    if (!this->hasSeenHero) {
        this->hp = 5;
        return;
    }

    // A Position object hPos is created to store the position of the hero obtained from hero.getPos()
    Position hPos = hero.getPos();

    this->hp--;

    // If the hp of the bomber reaches 0 after the decrement
    if (this->hp == 0) {
        Position sPos = this->getPos();

        // dx and dy is the absolute differences in the x and y coordinates between the hero's position (hPos) and the bomber's position (sPos)
        int dx = abs(hPos.x - sPos.x);
        int dy = abs(hPos.y - sPos.y);

        // If the distance between the hero and the bomber is 1 or less in either the x or y direction
        if (dx <= 1 || dy <= 1) {
            // Explosion sound effect
            this->music->setMedia(QUrl("qrc:/sound/sound/bomb.mp3"));
            this->music->play();

            // Damage hero
            hero.damage(this->power);
        }
    }
}
