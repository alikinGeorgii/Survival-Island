#include "expeditions.h"
#include <algorithm>
#include <cstdlib>

int planks = 0;

static int staminaCost(Location loc) {
  if (loc == Location::Wreck)
    return 20;
  return 8; // еда (джунгли) и вода (скалы)
}

std::wstring doExpedition(Player &player, Location loc) {
  int cost = staminaCost(loc);
  if (player.stamina < cost)
    return L"Недостаточно стамины";

  player.stamina -= cost;
  int roll = rand() % 100;
  std::wstring result;

  if (loc == Location::Wreck) { // обломки: безопасно, средняя добыча
    if (roll < 50) {
      int f = rand() % 15 + 5, w = rand() % 10 + 5;
      player.food += f;
      player.water += w;
      result = L"Нашёл припасы: еда +" + std::to_wstring(f) +
               L", вода +" + std::to_wstring(w);
    } else if (roll < 60) {
      int s = rand() % 20 + 10;
      player.stamina += s;
      result = L"Нашёл какао-бобы: выносливость +" + std::to_wstring(s);
    } else if (roll < 85) {
      result = L"Обломки пусты, ничего полезного";
    } else {
      player.hp -= 10;
      result = L"Порезался о металл: HP -10";
    }
  } else if (loc == Location::Jungle) { // джунгли: много еды, риск выше
    if (roll < 55) {
      int f = rand() % 20 + 10;
      player.food += f;
      result = L"Нашёл фрукты: еда +" + std::to_wstring(f);
    } else if (roll < 65) {
      int s = rand() % 30 + 15;
      player.stamina += s;
      result = L"Нашёл какао-бобы: выносливость +" + std::to_wstring(s);
    } else if (roll < 75) {
      result = L"Заблудился и вернулся ни с чем";
    } else {
      player.hp -= 20;
      result = L"Укусила змея: HP -20";
    }
  } else { // скалы: вода, риск падения
    if (roll < 50) {
      int w = rand() % 20 + 10;
      player.water += w;
      result = L"Нашёл ручей: вода +" + std::to_wstring(w);
    } else if (roll < 75) {
      result = L"Ничего не нашёл";
    } else {
      player.hp -= 15;
      result = L"Сорвался со скалы: HP -15";
    }
  }

  if (loc == Location::Wreck && rand() % 100 < 30) { // 30% найти доску
    planks++;
    result += L". Нашёл доску (" + std::to_wstring(planks) + L"/" +
              std::to_wstring(PLANKS_FOR_RAFT) + L")";
  }

  player.food = std::clamp(player.food, 0, 100);
  player.water = std::clamp(player.water, 0, 100);
  player.hp = std::clamp(player.hp, 0, 100);
  player.stamina = std::clamp(player.stamina, 0, 100);
  return result;
}

std::wstring randomEvent(Player &player) {
  int roll = rand() % 100;
  std::wstring result;

  if (roll < 20) {
    player.hp -= 10;
    player.water += 15;
    result = L"Ночью разыгрался шторм: HP -10, но набралась дождевая вода +15";
  } else if (roll < 40) {
    int f = rand() % 10 + 5;
    player.food += f;
    result = L"Море выбросило на берег рыбу: еда +" + std::to_wstring(f);
  } else if (roll < 55) {
    player.hp -= 10;
    player.stamina -= 15;
    result = L"Ты заболел: HP -10, стамина -15";
  } else if (roll < 70) {
    player.water += 20;
    result = L"Прошёл ливень: вода +20";
  } else if (roll < 80) {
    player.food -= 10;
    result = L"Чайки растащили часть запасов: еда -10";
  } else {
    result = L"Спокойная ночь, ничего не произошло";
  }

  player.food = std::clamp(player.food, 0, 100);
  player.water = std::clamp(player.water, 0, 100);
  player.hp = std::clamp(player.hp, 0, 100);
  player.stamina = std::clamp(player.stamina, 0, 100);
  return result;
}
