#include "expeditions.h"
#include "state.h"
#include "survival.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>

enum class GameState { Menu, Survival, Expedition };

void mainMenu(sf::RenderWindow &window, GameState &state) {
  static const sf::Texture background("../assets/mainMenu.jpg");
  static sf::Sprite bg(background);

  static sf::RectangleShape button1({200.0f, 50.0f});
  static sf::Font font("../assets/Oswald.ttf");
  static sf::Text play(font, L"Играть");
  button1.setPosition({550.f, 350.f});
  button1.setFillColor(sf::Color::White);
  button1.setOutlineThickness(-1.5f);
  button1.setOutlineColor(sf::Color::Black);
  play.setPosition({610.f, 355.f});
  play.setFillColor(sf::Color::Black);

  // клик: пиксели окна -> координаты мира, проверка по реальным границам кнопки
  static bool wasPressed = false;
  bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
  if (pressed && !wasPressed) {
    sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if (button1.getGlobalBounds().contains(pos))
      state = GameState::Survival;
  }
  wasPressed = pressed;

  window.draw(bg);
  window.draw(button1);
  window.draw(play);
}

void gameInterface(sf::RenderWindow &window, const Player &player) {
  static sf::Font font("../assets/Oswald.ttf");
  sf::Text info(font, L"День: " + std::to_wstring(player.day) + L"\nHP: " +
                          std::to_wstring(player.hp) + L"\nЕда: " +
                          std::to_wstring(player.food) + L"\nВода: " +
                          std::to_wstring(player.water) + L"\nСтамина: " +
                          std::to_wstring(player.stamina));
  info.setPosition({20.f, 20.f});
  window.draw(info);
}

void runSurvival(sf::RenderWindow &window, GameState &state, Player &player) {

  static const sf::Texture backg("../assets/beach.jpg");
  static sf::Sprite bg(backg);
  static sf::Font font("../assets/Oswald.ttf");
  // кнопка: поиск еды
  static sf::RectangleShape foodBtn({200.f, 50.f});
  static sf::Text foodText(font, L"Искать еду");
  foodBtn.setPosition({1050.f, 200.f});
  foodBtn.setFillColor(sf::Color::White);
  foodBtn.setOutlineThickness(-1.5f);
  foodBtn.setOutlineColor(sf::Color::Black);
  foodText.setPosition({1070.f, 205.f});
  foodText.setFillColor(sf::Color::Black);

  // кнопка: поиск воды
  static sf::RectangleShape waterBtn({200.f, 50.f});
  static sf::Text waterText(font, L"Искать воду");
  waterBtn.setPosition({1050.f, 300.f});
  waterBtn.setFillColor(sf::Color::White);
  waterBtn.setOutlineColor(sf::Color::Black);
  waterBtn.setOutlineThickness(-1.5f);
  waterText.setPosition({1070.f, 305.f});
  waterText.setFillColor(sf::Color::Black);

  // кнопка: обыск обломков
  static sf::RectangleShape trsBtn({250.f, 50.f});
  static sf::Text trsText(font, L"Обыскать обломки");
  trsBtn.setPosition({1030.f, 400.f});
  trsBtn.setFillColor(sf::Color::White);
  trsBtn.setOutlineThickness(-1.5f);
  trsBtn.setOutlineColor(sf::Color::Black);
  trsText.setPosition({1050.f, 405.f});
  trsText.setFillColor(sf::Color::Black);

  // кнопка: перемотка/конец времени/дня
  static sf::RectangleShape timeBtn({250.f, 50.f});
  static sf::Text timeTxt(font, L"");
  timeTxt.setString(player.time == 3 ? L"Завершить день" : L"Подождать");
  timeBtn.setPosition({500.f, 650.f});
  timeBtn.setFillColor(sf::Color::White);
  timeBtn.setOutlineThickness(-1.5f);
  timeBtn.setOutlineColor(sf::Color::Black);
  timeTxt.setPosition({520.f, 655.f});
  timeTxt.setFillColor(sf::Color::Black);

  static bool wasPressed = false;
  static std::wstring lastMessage = L"";
  bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
  if (pressed && !wasPressed) {
    sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if (foodBtn.getGlobalBounds().contains(pos) && !player.foodDone) {
      lastMessage = doExpedition(player, Location::Jungle);
      player.foodDone = true;
    }
    if (waterBtn.getGlobalBounds().contains(pos) && !player.waterDone) {
      lastMessage = doExpedition(player, Location::Rocks);
      player.waterDone = true;
    }
    if (trsBtn.getGlobalBounds().contains(pos) && !player.trsDone) {
      lastMessage = doExpedition(player, Location::Wreck);
      player.trsDone = true;
    }
    if (timeBtn.getGlobalBounds().contains(pos)) {
      if (player.time != 3) {
        player.time++;

        player.food -= rand() % 9 + 1;
        player.water -= rand() % 9 + 1;
        if (player.food <= 0 || player.water <= 0) {
          player.hp -= 15;
        }
        player.stamina += rand() % 10 + 5;
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
      player.trsDone = false;
      player.waterDone = false;
    }
    player.food = std::clamp(player.food, 0, 100);
    player.water = std::clamp(player.water, 0, 100);
    player.hp = std::clamp(player.hp, 0, 100);
    player.stamina = std::clamp(player.stamina, 0, 100);
  }
  wasPressed = pressed;
  foodBtn.setFillColor(player.foodDone ? sf::Color(120, 120, 120)
                                       : sf::Color::White);
  waterBtn.setFillColor(player.waterDone ? sf::Color(120, 120, 120)
                                         : sf::Color::White);
  trsBtn.setFillColor(player.trsDone ? sf::Color(120, 120, 120)
                                     : sf::Color::White);
  window.draw(bg);
  gameInterface(window, player);

  window.draw(foodBtn);
  window.draw(foodText);

  window.draw(waterBtn);
  window.draw(waterText);

  window.draw(trsBtn);
  window.draw(trsText);

  static sf::Text msg(font, L"");
  msg.setString(lastMessage);
  msg.setPosition({20.f, 300.f});
  msg.setFillColor(sf::Color::White);
  window.draw(msg);
  window.draw(timeBtn);
  window.draw(timeTxt);
}

int main() {
  srand(time(nullptr));

  sf::RenderWindow window(sf::VideoMode({1280, 720}), "Survival Game");
  window.setFramerateLimit(60);

  Player player;

  GameState cur_state = GameState::Menu;

  while (window.isOpen()) {
    // Process events
    while (const std::optional event = window.pollEvent()) {
      // Close window: exit
      if (event->is<sf::Event::Closed>())
        window.close();
    }
    // Clear screen
    window.clear();

    if (cur_state == GameState::Menu) {
      mainMenu(window, cur_state);
    } else if (cur_state == GameState::Survival) {
      runSurvival(window, cur_state, player);
    }

    // Draw the sprite
    window.display();
  }
  return 0;
}
