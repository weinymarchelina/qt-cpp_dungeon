#pragma once
#include "Creature.h"
#include "Hero.h"
#include <QMediaPlayer>

class Bomber : public Creature {

public:
    Bomber();

    void updateDamage(Hero& hero) override;

    // Music player
    QMediaPlayer* music = new QMediaPlayer;
};
