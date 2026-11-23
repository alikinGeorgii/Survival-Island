#pragma once
#include "state.h"
#include <string>

enum class Location { Wreck, Jungle, Rocks };

// выполняет вылазку: меняет параметры игрока, возвращает текст результата для экрана
std::wstring doExpedition(Player &player, Location loc);

// Случайное событие в конце дня: меняет параметры игрока, возвращает текст для экрана
std::wstring randomEvent(Player &player);

// Доски для концовки «плот»
extern int planks;
const int PLANKS_FOR_RAFT = 3;

// Случайное событие в конце дня: меняет параметры игрока, возвращает текст для экрана
std::wstring randomEvent(Player &player);
