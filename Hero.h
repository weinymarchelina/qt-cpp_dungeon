#pragma once
#include <QString>
#include "Creature.h"
#include "Item.h"

class Creature;
class Item;

#include <QVector>
#include <QDebug>
#include <QMediaPlayer>
#include "Position.h"

class Hero {

private:
	Position	sPos;			// Hero location
	char sIcon = 'H';	// Hero icon
	int	hp;
	int	sLevel = 1;     // Level
	int	sMaxExp;		// Level update experience
	int	sCurrentExp;	// current exp hero has
	int direction;
	int attack;
    QString sImgUrl;

public:
	// Default constructor
	Hero();

	// Setting constructor
	Hero(int x, int y, int level = 1, int maxExp = 15, int currentExp = 0, char icon = 'H', int hp = 100);

	// Setting constructor
	Hero(Position& pos, int level, int maxExp, int currentExp, char icon, int hp);

	// Set position
	void setPos(Position pos);
	void setPos(int x, int y);

	// Set icon
	void setIcon(char& icon);

	// Set level
	void setLevel(int level);

	// Set exp
	void setExp(int exp);

	// Set MaxExp
	void SetMaxExp(int maxexp);

	// Get position
	Position getPos(void);

	// Get Icon
	char getIcon(void);

	// Get level
	int getLevel(void);

	// Get Exp
	int getExp(void);

	// Get Max
	int getMaxExp(void);

	// Get HP
	int getHP();

	// Set HP
    void setHP(int hp);

	// Calculate Gain of EXP
	void gainEXP(int points);

	// Reduce user HP by damage
	void damage(int points);

	// Swallow ability
    void swallow(int width, int height, QVector<Creature*>& creatures, QVector<Item*>& items, int foodBoost);

	// Get hero direction
	int getDirection();

	// Set hero direction
	void setDirection(int direction);

	// Get hero attack
	int getAttack();

	// Set hero attack
	void setAttack(int attack);

    // Get hero img
    QString getImgUrl();

    // Get hero eat range
    QVector<Position> getEatRange();

    // Music player
    QMediaPlayer* music = new QMediaPlayer;
};
