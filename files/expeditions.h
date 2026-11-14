#pragma once
#include "state.h"
#include <string>

enum class Location { Wreck, Jungle, Rocks };

// выполняет вылазку: меняет параметры игрока, возвращает текст результата для экрана
std::wstring doExpedition(Player &player, Location loc);
