#include "expeditions.h"
#include "state.h"
#include "survival.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>

enum class GameState { Menu, Survival, Expedition, GameOver, Victory };

static std::wstring lastMessage = L"";

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
                          std::to_wstring(player.stamina) + L"\nДоски: " +
                          std::to_wstring(planks) + L"/" +
                          std::to_wstring(PLANKS_FOR_RAFT));
  info.setPosition({20.f, 20.f});
  window.draw(info);
}

void runSurvival(sf::RenderWindow &window, GameState &state, Player &player) {

  static const sf::Texture bgM("../assets/beach.jpg");
  static const sf::Texture bgA("../assets/beachDay.jpg");
  static const sf::Texture bgE("../assets/beachEvening.jpg");
  static sf::Sprite bg(bgM);
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
      advanceTime(player);
      if (player.time != 1)
        player.stamina = std::clamp(player.stamina + rand() % 10 + 5, 0, 100);
      else
        lastMessage = randomEvent(player);
    }
  }

  if (player.hp <= 0)
    state = GameState::GameOver;
  else if (player.day > 5)
    state = GameState::Victory;

  wasPressed = pressed;
  foodBtn.setFillColor(player.foodDone ? sf::Color(120, 120, 120)
                                       : sf::Color::White);
  waterBtn.setFillColor(player.waterDone ? sf::Color(120, 120, 120)
                                         : sf::Color::White);
  trsBtn.setFillColor(player.trsDone ? sf::Color(120, 120, 120)
                                     : sf::Color::White);
  if (player.time == 1) {
    bg.setTexture(bgM);
  } else if (player.time == 2) {
    bg.setTexture(bgA);
  } else {
    bg.setTexture(bgE);
  }
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

void runEnd(sf::RenderWindow &window, GameState &state, Player &player,
            bool win) {
  static const sf::Texture backg("../assets/beach.jpg");
  static sf::Sprite bg(backg);
  static sf::Font font("../assets/Oswald.ttf");

  static sf::RectangleShape dark({1280.f, 720.f});
  dark.setFillColor(sf::Color(0, 0, 0, 160));

  static sf::Text title(font, L"", 60);
  title.setString(win ? L"Вы спаслись!" : L"Вы погибли");
  title.setPosition({480.f, 250.f});
  title.setFillColor(win ? sf::Color::Green : sf::Color::Red);

  static sf::Text info(font, L"", 30);
  info.setString(L"Прожито дней: " + std::to_wstring(player.day - 1));
  info.setPosition({520.f, 340.f});
  info.setFillColor(sf::Color::White);

  static sf::RectangleShape menuBtn({200.f, 50.f});
  static sf::Text menuText(font, L"В меню");
  menuBtn.setPosition({540.f, 450.f});
  menuBtn.setFillColor(sf::Color::White);
  menuBtn.setOutlineThickness(-1.5f);
  menuBtn.setOutlineColor(sf::Color::Black);
  menuText.setPosition({600.f, 455.f});
  menuText.setFillColor(sf::Color::Black);

  static bool wasPressed = false;
  bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
  if (pressed && !wasPressed) {
    sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if (menuBtn.getGlobalBounds().contains(pos)) {
      player = Player(); // сброс игрока: все статы и флаги по умолчанию
      planks = 0;
      lastMessage = L"";
      state = GameState::Menu;
    }
  }
  wasPressed = pressed;

  window.draw(bg);
  window.draw(dark);
  window.draw(title);
  window.draw(info);
  window.draw(menuBtn);
  window.draw(menuText);
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
    } else if (cur_state == GameState::GameOver) {
      runEnd(window, cur_state, player, false);
    } else if (cur_state == GameState::Victory) {
      runEnd(window, cur_state, player, true);
    }

    // Draw the sprite
    window.display();
  }
  return 0;
}
