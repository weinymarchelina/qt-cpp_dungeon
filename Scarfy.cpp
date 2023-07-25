/***********************************************************************
 * Author:  B11115010 鍾詩靈
 *          B11115014 魏美芳
 * Create Date: 2023/05/24
 * Update Date: 2023/06/01
 * Description: This C++ program performs dungeon game with kirby theme that have many features (see readme file)
***********************************************************************/

#include "Scarfy.h"

//The default constructor of the Scarfy class initializes the member variables
Scarfy::Scarfy() {
    this->sIcon = 'S';
    this->power = 5;
    this->viewDistance = 10;
    this->chaseHeroProbability = 70;
    this->awareness = true;
    this->hp = 5;
    this->sImgUrl = "_scarfy.PNG";
}

// Generate the image URL for the Scarfy based on its state
QString Scarfy::getImgUrl() {
    // Check if the Scarfy has seen the hero
    if (this->hasSeenHero) {
        this->sImgUrl = "_mutant.PNG";
    }
    else {
        this->sImgUrl = "_scarfy.PNG";
    }

    // Generate the URL based on the direction and image URL
    QString url = ":/img/img/" + QString::number(this->direction) + this->sImgUrl;

    return url;
}
