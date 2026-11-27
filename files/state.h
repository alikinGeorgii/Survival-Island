#ifndef STATE_H
#define STATE_H
struct Player {
  int hp = 100, food = 100, water = 100, stamina = 100, day = 1;
  bool foodDone = false;
  bool waterDone = false;
  bool trsDone = false;
  int time = 1;
};

//смена времени и суток
void advanceTime(Player &player);

//проверка конца игры
bool checkGameOver(const Player &player);
#endif // !STATE_H
