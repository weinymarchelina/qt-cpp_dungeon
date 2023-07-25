#pragma once
#include <QString>
#include <QVector>
#include "Position.h"
#include "Hero.h"

class Hero;

class Creature {//creature class 
protected:
	Position	sPos;			// Creature position
    char sIcon = 'C';
	bool hasSeenHero;
	int power;
	int viewDistance;
    Position hPos;
	Position heroPrevPos;
	int heroPrevDir;
    int chaseHeroProbability;
    bool canMove = true;
    bool awareness = false;
	int hp;
    void updateSight();
	virtual void updateDamage(Hero& hero);
    void chaseHero(int prevHeroDir);
    void moveRandomly();
    QString sImgUrl;
	int direction;
    bool checkCanSee(Position dir);
    float clip(float n, float minimum, float maximum);
    bool checkPositionValid(Position pos);
    int gWidth = -1;
    int gHeight = -1;
    QVector<Position> filledPositions;


public:
	// Default constructor
	Creature();
	void setPos(Position pos);
	void setPos(int x, int y);
	void setIcon(char& icon);
	Position getPos(void);
	char getIcon(void);
	bool getHasSeen();
	int getHealth();
	void setHealth(int);
    virtual QString getImgUrl();
    void setImgUrl(QString url);
	virtual void update(Hero& hero);
	int getDirection();
	void setDirection(int direction);
    void setWidthHeight(int width, int height);
    void setFilledPositions(QVector<Position> filledPositions);
    void setHasSeen(bool hasSeen);
};

