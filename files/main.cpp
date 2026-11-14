#include <SFML/Graphics.hpp>

enum class GameState { Menu, Survival, Expedition };

void mainMenu(sf::RenderWindow &window, GameState &state) {
  static const sf::Texture background("../assets/mainMenu.jpg");
  static sf::Sprite bg(background);
  window.draw(bg);

  sf::RectangleShape button1({200.0f, 50.0f});
  button1.setPosition({550.f, 350.f});
  button1.setFillColor(sf::Color::White);
  button1.setOutlineThickness(-1.5f);
  button1.setOutlineColor(sf::Color::Black);
  window.draw(button1);
}

void runSurvival(sf::RenderWindow &window, GameState &state) {}

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
