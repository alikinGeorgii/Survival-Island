#include "state.h"
#include <cstdlib> 
#include <algorithm>

//смена времени и суток
void advanceTime(Player &player) { 
  if (player.time != 3) { player.time++;
  player.food -= rand() % 9 + 1;
  player.water -= rand() % 9 + 1;

  if (player.food <= 0 || player.water <= 0) {
    player.hp -= 15;
  }
} else {
  player.time = 1;
  player.day++;

  player.food -= rand() % 15 + 1;
  player.water -= rand() % 15 + 1;

  if (player.food <= 0 || player.water <= 0) {
    player.hp -= 30;
  }

  player.stamina += rand() % 30 + 20;
}

player.foodDone = false;
player.waterDone = false;
player.trsDone = false;

player.food = std::clamp(player.food, 0, 100);
player.water = std::clamp(player.water, 0, 100);
player.hp = std::clamp(player.hp, 0, 100);
player.stamina = std::clamp(player.stamina, 0, 100);
}

//проверка конца игры
bool checkGameOver(const Player &player) {
  if (player.hp <= 0 || player.day >= 5) return true;
  return false;
}
