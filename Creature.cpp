/***********************************************************************
 * Author:  B11115010 鍾詩靈
 *          B11115014 魏美芳
 * Create Date: 2023/05/24
 * Update Date: 2023/06/01
 * Description: This C++ program performs dungeon game with kirby theme that have many features (see readme file)
***********************************************************************/

#include "Creature.h"
#include <QDebug>

// Default constructor
Creature::Creature()
{
    this->sPos.x = -1;
    this->sPos.y = -1;
    this->sIcon = 'C';
    this->power = 2;
    this->hasSeenHero = false;
    this->viewDistance = 3;
    this->chaseHeroProbability = 0;
    this->heroPrevPos = Position(-1, -1);
    this->heroPrevDir = 0;
    this->canMove = true;
    this->awareness = false;
    this->hp = 1;
    this->direction = rand() % 4;
    this->sImgUrl = "_dee.PNG";
}

// Set position
void Creature::setPos(Position pos)
{
    this->sPos = pos;
    this->heroPrevPos = pos;
}

// Set position for x y parameter
void Creature::setPos(int x, int y)
{
    this->sPos.x = x;
    this->sPos.y = y;
}

// Set icon
void Creature::setIcon(char& icon)
{
    this->sIcon = icon;
}

// Get position
Position Creature::getPos()
{
    return this->sPos;
}

// Get Icon
char Creature::getIcon()
{
    return this->sIcon;
}

// Get has seen
bool Creature::getHasSeen() {
    return this->hasSeenHero;
}

// Get creature's HP
int Creature::getHealth() {
    return this->hp;
}

// Set creature's HP
void Creature::setHealth(int hp) {
    this->hp = hp;
}

// Update creature state
void Creature::update(Hero& hero)
{
    // Updtae hPos
    this->hPos = hero.getPos();

    // Check and update creature sight state
    this->updateSight();

    // Let the creature chase the hero if it sees the hero, otherwise let it move randomly
    if (this->hasSeenHero) {
        // Calculate the probability of chasing the hero
        int chaseProbability = rand() % 101;

        // If the chase probability is less than or equal to the chaseHeroProbability, chase the hero
        if (chaseProbability <= this->chaseHeroProbability) {
            this->chaseHero(hero.getDirection());
        }
         // If the creature can move, but it doesn't chase the hero, move randomly
        else if (this->canMove) {
            this->moveRandomly();
        }
    }
    else if (this->canMove) {
        // Move randomly
        this->moveRandomly();
    }

    // Update damage to the hero
    this->updateDamage(hero);

    // If the position of the creature is the same as the position of hero, let it move randomly till the position is not the same
    while (hPos == sPos) {
        this->moveRandomly();
    }
}


// Update Sight
void Creature::updateSight()
{
    Position dir;

    // Check if the creature can see in a specific direction and if it has awareness
    if (this->checkCanSee(dir) && this->awareness)
    {
        this->hasSeenHero = true;
    }
    else
    {
        this->hasSeenHero = false;
    }
}

// Update damage to hero
void Creature::updateDamage(Hero& hero)
{
    // Get the position of the hero
    Position hPos = hero.getPos();

    // If the position of the creature is the same as the position of the hero, damage the hero
    if (hPos == sPos)
    {
        hero.damage(power);
        this->moveRandomly();
    }
}

// Move randomly
void Creature::moveRandomly() {
    // Move the creature randomly to up, down, left or right and test whether the new position is valid
    int randomNum = rand() % 4;

    // Create a new position based on the current position and the random direction
    Position newPosition = sPos;

    // Update the new position and direction based on the random number
    if (randomNum == 0) {
        newPosition.x += 1;
        this->direction = 1;
    }
    else if (randomNum == 1) {
        newPosition.x -= 1;
        this->direction = 3;
    }
    else if (randomNum == 2) {
        newPosition.y += 1;
        this->direction = 2;
    }
    else if (randomNum == 3) {
        newPosition.y -= 1;
        this->direction = 0;
    }

    // Check if the new position is valid
    if (checkPositionValid(newPosition)) {
        sPos = newPosition;
    }
}

// Update move
void Creature::chaseHero(int _prevHeroDir) {
    // Check if the hero is within viewRange steps distance from the creature
    int dx = abs(sPos.x - this->hPos.x);
    int dy = abs(sPos.y - this->hPos.y);

    // Calculate the new position of the hero and the creature
    int newDx = abs(sPos.x - this->heroPrevPos.x);
    int newDy = abs(sPos.y - this->heroPrevPos.y);

    // Check if the hero and the creature overlap in the current position
    bool overlapsHero = (this->hPos.x == sPos.x && this->hPos.y == sPos.y);

    // if the hero is within 1 step range of the creature on diagonal axis, let the creature move to the hero's previous spot to prevent the hero being blocked by the creature on a specific axis
    if (newDx == 1 && newDy == 1 && dx == 1 && dy == 1 && !overlapsHero && checkPositionValid(this->heroPrevPos))
    {
        this->sPos = this->heroPrevPos;
        this->direction = this->heroPrevDir;
    }
    else
    {
        // let the creature move towards hero
        if (this->hPos.x - sPos.x > 0) {
            dx = 1;
            this->direction = 1;
        }
        else if (this->hPos.x - sPos.x < 0) {
            dx = -1;
            this->direction = 3;
        }
        else {
            dx = 0;
        }

        if (this->hPos.y - sPos.y > 0) {
            dy = 1;
            this->direction = 2;
        }
        else if (this->hPos.y - sPos.y < 0) {
            dy = -1;
            this->direction = 0;
        }
        else {
            dy = 0;
        }

        // test whether the new position of the creature is valid
        Position newPosition = sPos;
        newPosition.y += dy;
        newPosition.x += dx;

        // Check whethr many things surround hero so that hero still can escape
        int surrounders = 0;

        QVector<Position> delta;
        delta.push_back(Position(0, -1));
        delta.push_back(Position(0, 1));
        delta.push_back(Position(-1, 0));
        delta.push_back(Position(1, 0));

        for (int i = 0; i < 4 && surrounders < 2; i++) {
            Position testPos = this->hPos;

            // Update test position based on delta movement
            testPos += delta[i];

             // Check if the test position is not valid (blocked or out of bounds)
            if (!checkPositionValid(testPos)) {
                surrounders++;
            }
        }

        // if less than 2 surrounders then check new position
        if (surrounders < 2) {
            // if the new position is valid, then set the creature's position to its new position
            if (checkPositionValid(newPosition))
            {
                this->sPos = newPosition;
            }
        }
    }

    // save hero's previous position
    this->heroPrevPos = this->hPos;
    this->heroPrevDir = _prevHeroDir;
}

// Get the creature direction
int Creature::getDirection() { return this->direction; }

// Set the creature direction
void Creature::setDirection(int direction) { this->direction = direction; }

// Clip algorithm
float Creature::clip(float n, float minimum, float maximum)
{
    return std::max(minimum, std::min(n, maximum));
}

// Can see function
bool Creature::checkCanSee(Position dir) {
    // the dir_x and dir_y value are call-by-refernce
    dir.x = (int)this->clip((float)(hPos.x - this->sPos.x), -1.f, 1.f); // clip the value between -1 ~ 1
    dir.y = (int)this->clip((float)(hPos.y - this->sPos.y), -1.f, 1.f);

    // Integer variable count is initialized to 0
    int count = 0;

    // Checks if the position cPos plus the direction dir multiplied by count is equal to the target position hPos
    do {
        // spot the target position
        if (this->sPos + dir * count == this->hPos) {
            return true;
        }

        count++;
    } while (count < this->viewDistance); // check the range in 4 units

    return false;
}

// Check valid pos
bool Creature::checkPositionValid(Position pos) {
    // Check if the position is the same as the hero's position
    if (this->hPos == pos) {
        return true;
    }

    // Check if the position is within the game boundaries
    if (!pos.isInRange(this->gWidth, this->gHeight)) {
        return false;
    }

    // Check if the position is already occupied by other filled positions
    for (Position& filledPos : this->filledPositions) {
        // Compare the position with each filled position
        if (pos == filledPos) {
            return false;
        }
    }

    return true;
}

// Set width and height
void Creature::setWidthHeight(int width, int height) {
    this->gWidth = width;
    this->gHeight = height;
}

// Set filled positions
void Creature::setFilledPositions(QVector<Position> filledPositions) {
    this->filledPositions = filledPositions;
}

// Get image
QString Creature::getImgUrl() {
    // Construct the image URL based on the direction and image URL of the creature
    QString url = ":/img/img/" + QString::number(this->direction) + this->sImgUrl;

    // Return the image URL
    return url;
}

// Set has seen
void Creature::setHasSeen(bool hasSeen) {
    this->hasSeenHero = hasSeen;
}
