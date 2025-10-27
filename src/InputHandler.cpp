#include "InputHandler.hpp"
#include <iostream>

void InputHandler::handleKeyPress() const
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Escape))
        _window.close();
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
        _player.movePlayerLeft(true);
    else
        _player.movePlayerLeft(false);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
        _player.movePlayerRight(true);
    else
        _player.movePlayerRight(false);
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space))
        _player.shoot();
    
}