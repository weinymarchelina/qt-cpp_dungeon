/***********************************************************************
 * Author:  B11115010 鍾詩靈
 *          B11115014 魏美芳
 * Create Date: 2023/05/24
 * Update Date: 2023/06/01
 * Description: This C++ program performs dungeon game with kirby theme that have many features (see readme file)
***********************************************************************/

#include "Hero.h"

// Default constructor
Hero::Hero() {
	this->sPos.x = 0;
	this->sPos.y = 0;
	this->sLevel = 1;
	this->sCurrentExp = 0;
	this->sMaxExp = 15;
	this->sIcon = 'H';
    this->hp = 100;
    this->direction = rand() % 4;
    this->attack = 1;
    this->sImgUrl = "_kirby.PNG";
};

// Setting constructor
Hero::Hero(int x, int y, int level, int maxExp, int currentExp, char icon, int hp) {
	this->sPos.x = x;
	this->sPos.y = y;
	this->sLevel = level;
	this->sCurrentExp = currentExp;
	this->sMaxExp = maxExp;
	this->sIcon = icon;
	this->hp = hp;
    this->direction = rand() % 4;
    this->attack = 1;
    this->sImgUrl = "_kirby.PNG";
};

// Setting constructor
Hero::Hero(Position& pos, int level, int maxExp, int currentExp, char icon, int hp) {
	this->sPos = pos;
	this->sLevel = level;
	this->sCurrentExp = currentExp;
	this->sMaxExp = maxExp;
	this->sIcon = icon;
	this->hp = hp;
    this->direction = rand() % 4;
    this->attack = 1;
    this->sImgUrl = "_kirby.PNG";
};

// Set position for position parameter
void Hero::setPos(Position pos) { this->sPos = pos; }

// Set position for int x and int y parameter
void Hero::setPos(int x, int y) {
	this->sPos.x = x;
	this->sPos.y = y;
}

// Set icon
void Hero::setIcon(char& icon) { this->sIcon = icon; }

// Set level
void Hero::setLevel(int level) { this->sLevel = level; }

// Set exp
void Hero::setExp(int exp) { this->sCurrentExp = exp; }

// Set MaxExp
void Hero::SetMaxExp(int maxexp) { this->sMaxExp = maxexp; }

// Get position
Position Hero::getPos(void) { return this->sPos; }

// Get Icon
char Hero::getIcon(void) { return this->sIcon; }

// Get level
int Hero::getLevel(void) { return this->sLevel; }

// Get Exp
int Hero::getExp(void) { return this->sCurrentExp; }

// Get Max
int Hero::getMaxExp(void) { return this->sMaxExp; }

// Get HP
int Hero::getHP() { return this->hp; }

// Set HP
void Hero::setHP(int hp) { this->hp = hp; }

//The function of the Hero class takes an integer points parameter and increments the hero's experience points (sCurrentExp)
void Hero::gainEXP(int points) {
	//Intialized number as points
	int number = points;

    // While number is greater than 0, continue process the exp
	while (number > 0) {

		//If the sum of the current experience points and number is greater than or equal to the maximum experience points (sMaxExp), the hero levels up
		if (sCurrentExp + number >= sMaxExp) {//level up
			number -= (sMaxExp - sCurrentExp);
			sCurrentExp = 0;
			sMaxExp = (int)((float)sMaxExp * 1.2f);
			sLevel++;
		}
		//If the sum of the current experience points and number is less than sMaxExp
		else {
			sCurrentExp += number;
			number = 0;
		}
	}

	// Update attack based on the current level
    this->attack = 1.25 * sLevel;

    // Gain exp sound effect
    this->music->setMedia(QUrl("qrc:/sound/sound/exp.wav"));
    this->music->play();
}

// The function of the Hero class takes an integer points parameter and reduces the hero's hit points (hp) by points
void Hero::damage(int points) {
	hp = hp - points;

	//If hp becomes less than 0, it is set to 0 to avoid negative hit points
	if (hp < 0) {
		hp = 0;
	}

    // Damage sound effect
    this->music->setMedia(QUrl("qrc:/sound/sound/dmg_" + QString::number(rand() % 4) + ".wav"));
    this->music->play();
}

// Get eating range
QVector<Position> Hero::getEatRange() {
    // Set the eating range to 2 cells
    int range = 2;

    // Initialize the vector to store test cells
    QVector<Position> testCells;

    // Get the hero's direction
    int heroDir = this->direction;

    // Iterate over the range of cells
    for (int i = 1; i <= range; i++) {
        Position testPos = this->sPos;

        // Modify the cell according to the hero's direction
        if (heroDir == 0) {
            testPos.y -= i;
        }
        else if (heroDir == 1) {
            testPos.x += i;
        }
        else if (heroDir == 2) {
            testPos.y += i;
        }
        else if (heroDir == 3) {
            testPos.x -= i;
        }

        // Add the test position to the vector
        testCells.push_back(testPos);
    }

    return testCells;
}

// The function eats food or enemy that is located in front of the hero (limit two cells, based on direction)
void Hero::swallow(int width, int height, QVector<Creature*>& creatures, QVector<Item*>& items, int foodBoost) {
    // Swallow sound effect
    this->music->setMedia(QUrl("qrc:/sound/sound/eat.wav"));
    this->music->play();

    // Get the testCells of eating range
    QVector<Position> testCells = this->getEatRange();

    // Get the first and second cells in the eating range
    Position cell1 = testCells[0];
    Position cell2 = testCells[1];

	// Initialize new variable
	Position eatenCell = Position(-1, -1);
	char icon = ' ';

	// Check whether the second cell is valid to set it as eatenCell and get the icon
	if (cell2.isInRange(width, height)) {
		char targetIcon = ' ';

		// Find creature that is on cell2 position
		for (auto& creature : creatures) {
			if (creature->getPos() == cell2) {
				targetIcon = creature->getIcon();
				break;
			}
		}

		// If the are no creature on cell2, then find whether there are items on that position
		if (targetIcon == ' ') {

			// Find item that is on cell2 position
			for (auto& item : items) {
                // Check if the item's position matches cell2
				if (item->getPos() == cell2) {
					targetIcon = item->getIcon();
					break;
				}
			}
		}

		// If that position is not empty then set it as eaten cell
		if (targetIcon != ' ' && targetIcon != 'T') {
			icon = targetIcon;
			eatenCell = cell2;
		}
	}

	// Check whether the first cell is valid and overwites the eaten cell to the first one if the first cell also not empty
	if (cell1.isInRange(width, height)) {
		char targetIcon = ' ';

		// Find creature that is on cell1 position
		for (auto& creature : creatures) {
			if (creature->getPos() == cell1) {
				targetIcon = creature->getIcon();
				break;
			}
		}

		// If the are no creature on cell1, then find whether there is item on that position
		if (targetIcon == ' ') {

			// Find item that is on cell1 position
			for (auto& item : items) {
				if (item->getPos() == cell1) {
					targetIcon = item->getIcon();
					break;
				}
			}
		}

		// If that position is not empty trigger then set it as eaten cell
		if (targetIcon != ' ' && targetIcon != 'T') {
			icon = targetIcon;
			eatenCell = cell1;
		}
	}

	// If the eatenCell is not invalid, then handle the swallow
    if (eatenCell != Position(-1, -1)) {

		// Handle the swallow based on the swallowed class (item or enemy) from the eatenCell icon
        if (icon == 'C' || icon == 'S' || icon == 'B') {

			// Loop through the creatures to eat that enemy
			for (auto& creature : creatures) {

				// Check whether the creature is eatenCell creature and make sure that the hero's attack is higher than the creature's hp
                if (creature->getPos() == eatenCell && this->attack > creature->getHealth()) {
                    // Gain additional health when eating enemy (gain helath based on the enemy's current health)
                    this->hp += creature->getHealth();
                    if (this->hp > 100) this->hp = 100;

                    // Set the creature hp to 0 to be deleted
					creature->setHealth(0);

                    // Eating sound effect
                    this->music->setMedia(QUrl("qrc:/sound/sound/nyum.mp3"));
                    this->music->play();

					break;
				}
			}
		}
		else {
			// Loop though the items to eat that food
			for (auto& item : items) {

				// Check whether the item is the eaten cell and also make sure that the hero hp is not full
				if (item->getPos() == eatenCell && this->hp < 100) {
					// Handle different effect based on the type (tomato or not)
					if (item->getIcon() == 'M') {
						this->hp = 100;
					}
					else {
						// If the icon is not 'M', then it is a food, so just boost amount hp recovery
						int newHp = this->hp + foodBoost;

						// Set hp not exceeding 100
						if (newHp > 100) {
							newHp = 100;
						}

						// Set new hp to the hero's hp
						this->hp = newHp;
					}

					// Set isTriggered to true since the hero has eat the food
					item->setHasTriggered(true);
					break;
				}
			}
		}
	}
}

// Get the hero direction
int Hero::getDirection() { return this->direction; }

// Set the hero direction
void Hero::setDirection(int direction) { this->direction = direction; }

// Get hero attack
int Hero::getAttack() { return this->attack; }

// Set hero attack
void Hero::setAttack(int attack) { this->attack = attack; }

// Get image
QString Hero::getImgUrl() {
    // Construct the URL using the direction and image URL
    QString url = ":/img/img/" + QString::number(this->direction) + this->sImgUrl;

    // Return the constructed URL
    return url;
}
