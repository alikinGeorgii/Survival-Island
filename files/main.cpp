#include "state.h"
#include "survival.h"
#include <SFML/Graphics.hpp>

enum class GameState { Menu, Survival, Expedition };

void mainMenu(sf::RenderWindow &window, GameState &state) {
  static const sf::Texture background("../assets/mainMenu.jpg");
  static sf::Sprite bg(background);

  static sf::RectangleShape button1({200.0f, 50.0f});
  static sf::Font font("../assets/Oswald.ttf");
  static sf::Text play(font, L"Играть");
  static bool inited = false;
  if (!inited) {
    button1.setPosition({550.f, 350.f});
    button1.setFillColor(sf::Color::White);
    button1.setOutlineThickness(-1.5f);
    button1.setOutlineColor(sf::Color::Black);
    play.setPosition({610.f, 355.f});
    play.setFillColor(sf::Color::Black);
    inited = true;
  }

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

void runSurvival(sf::RenderWindow &window, GameState &state) {
  static const sf::Texture backg("../assets/beach.jpg");
  static sf::Sprite bg(backg);
  window.draw(bg);
}

int main() {
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
