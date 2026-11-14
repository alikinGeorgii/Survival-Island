#include "state.h"
#include "survival.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <string>

enum class GameState { Menu, Survival, Expedition };

int hp = 100, food = 100, water = 100, stamina = 100, day = 1;
static bool foodDone = false;

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

void gameInterface(sf::RenderWindow &window) {
  static sf::Font font("../assets/Oswald.ttf");
  sf::Text info(font, L"День: " + std::to_wstring(day) + L"\nHP: " +
                          std::to_wstring(hp) + L"\nЕда: " +
                          std::to_wstring(food) + L"\nВода: " +
                          std::to_wstring(water) + L"\nСтамина: " +
                          std::to_wstring(stamina));
  info.setPosition({20.f, 20.f});
  window.draw(info);
}

void runSurvival(sf::RenderWindow &window, GameState &state) {
  static const sf::Texture backg("../assets/beach.jpg");
  static sf::Sprite bg(backg);
  static sf::Font font("../assets/Oswald.ttf");
  static sf::RectangleShape foodBtn({200.f, 50.f});
  static sf::Text foodText(font, L"Искать еду");
  foodBtn.setPosition({1050.f, 200.f});
  foodBtn.setFillColor(sf::Color::White);
  foodBtn.setOutlineThickness(-1.5f);
  foodBtn.setOutlineColor(sf::Color::Black);
  foodText.setPosition({1070.f, 205.f});
  foodText.setFillColor(sf::Color::Black);

  static bool wasPressed = false;
  bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
  if (pressed && !wasPressed) {
    sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if (foodBtn.getGlobalBounds().contains(pos) && !foodDone) {
      food += rand() % 15 + 1;
      foodDone = true;
    }
  }
  wasPressed = pressed;
  foodBtn.setFillColor(foodDone ? sf::Color(120, 120, 120) : sf::Color::White);

  window.draw(bg);
  gameInterface(window);
  window.draw(foodBtn);
  window.draw(foodText);
}

int main() {
  srand(time(nullptr));

  sf::RenderWindow window(sf::VideoMode({1280, 720}), "Survival Game");

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
      runSurvival(window, cur_state);
    }

    // Draw the sprite
    window.display();
  }
  return 0;
}
