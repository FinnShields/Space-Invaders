#pragma once

#include "Player.hpp"
#include <SFML/Graphics.hpp>

class InputHandler
{
  public:
    InputHandler(sf::RenderWindow &window, Player &player) : _window(window), _player(player) {};
    ~InputHandler() {};
    void handleKeyPress();

  private:
    sf::RenderWindow &_window;
    Player &_player;
};