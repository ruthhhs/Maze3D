#ifndef CHEST_H
#define CHEST_H

#include <stdbool.h>

extern bool nearChest;
extern bool chestOpened;

extern float chestAngle;

void drawChest(float x, float y, float z);

void updateChest();

void checkChest(float playerX, float playerZ,
                float chestX, float chestZ);

void openChest();

#endif
