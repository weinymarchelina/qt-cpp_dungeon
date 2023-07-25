/***********************************************************************
 * Author:  B11115010 鍾詩靈
 *          B11115014 魏美芳
 * Create Date: 2023/05/24
 * Update Date: 2023/06/01
 * Description: This C++ program performs dungeon game with kirby theme that have many features (see readme file)
***********************************************************************/

#include "Food.h"

// The constructor initializes sBoost to 10 and sIcon to 'T
Food::Food(int tomatoRate) : sBoost(10) {
    sIcon = 'F';
    sTomatoRate = tomatoRate;
    this->sIsTriggered = false;
    this->generateFoodType();
}

// The constructor takes x and y as parameters, initializes sExp to 10, sets sPos.x to x, sPos.y to y, and sIcon to 'T'
Food::Food(int x, int y, int tomatoRate) : sBoost(10) {
    sPos.x = x;
    sPos.y = y;
    sIcon = 'F';
    sTomatoRate = tomatoRate;
    this->sIsTriggered = false;
    this->generateFoodType();
}

// The copy constructor creates a new Food object by copying the values from ref
Food::Food(const Food& ref) {
    *this = ref;
}

// The update function checks if hero's position matches Food's position and if so, the hero gains + 10 to its HP
void Food::update(Hero& hero) {
    // If the positions match, the hero gains + 10 hp recovery
    if (hero.getPos().x == sPos.x && hero.getPos().y == sPos.y && hero.getHP() < 100) {
        // If the icon is 'M' (stands for Maxim Tomato, an item that replenish all the hero's hp), then recover all the hero health to max
        if (this->sIcon == 'M') {
            hero.setHP(100);

        }
        else {
            // If the icon is not 'M', then it is a food, so +10 hp recovery
            int newHp = hero.getHP() + sBoost;

            // Set hp not exceeding 100
            if (newHp > 100) {
                newHp = 100;
            }

            // Set new hp to the hero's hp
            hero.setHP(newHp);
        }

        // Set isTriggered to true since the hero has step on the item
        this->sIsTriggered = true;
    }
}

// Get tomato rate
int Food::getTomatoRate() {
    return this->sTomatoRate;
}

// Set tomato rate
void Food::setTomatoRate(int rate) {
    this->sTomatoRate = rate;
}

// Generate food type
void Food::generateFoodType() {
    // Generate random number
    int foodProbability = rand() % 101;

    // Check tomato probability by comparing the random number to the tomato rate
    if (foodProbability <= this->sTomatoRate) {
        this->sIcon = 'M';

        this->sImgUrl = "M_food.PNG";
    }
    else {
        // Generate random number for the food probabiity
        foodProbability = rand() % 5;

        // Based on the number, get the icon
        switch (foodProbability) {
        case 0:
            this->sIcon = '0';
            break;

        case 1:
            this->sIcon = '1';
            break;

        case 2:
            this->sIcon = '2';
            break;

        case 3:
            this->sIcon = '3';
            break;

        case 4:
            this->sIcon = '4';
            break;
        }

        // Add the index to the img url
        this->sImgUrl = QString(this->sIcon) + "_food.png";
    }
}

// Get the boost
int Food::getBoost() {
    return this->sBoost;
}

// Set the boost
void Food::setBoost(int boost) {
    this->sBoost = boost;
}
