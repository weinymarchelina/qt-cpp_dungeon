#pragma once
#include "Creature.h"

class Scarfy : public Creature {
public:
    Scarfy();
    QString getImgUrl() override;
};
