#include "Game.hpp"
#include <algorithm>
#include <iostream>

Game::Game(sf::RenderWindow &window, Player &player, InputHandler &inputHandler, EnemyHandler& enemyHandler)
    : _window(window), _player(player), _inputHandler(inputHandler), _enemyHandler(enemyHandler)
{
    if (!_font.openFromFile("assets/Lavigne.ttf"))
    {
        std::cerr << "Font failed to load";
        _window.close();
    }
}

void Game::run()
{
    while (_window.isOpen())
    {
        processEvents();
        update();
        render();
    }
    std::cout << "Final score: " << _killCount << " kills. Nice work!" << std::endl;
}

void Game::processEvents()
{
    while (const std::optional event = _window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            _window.close();
    }
}

bool Game::collides(sf::RectangleShape shape1, sf::RectangleShape shape2)
{
    if (shape1.getGlobalBounds().findIntersection(shape2.getGlobalBounds()) != std::nullopt)
        return true;
    return false;
}

void Game::update()
{
    float dt = _clock.restart().asSeconds();
    _inputHandler.handleKeyPress();

    switch (_state)
    {
    case (START):
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter))
            _state = GAME;
        break;
    case (GAME):
        gameLoop(dt);
        break;
    case (END):
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter))
        {
            _state = GAME;
            resetGame();
        }
        break;
    default:
        break;
    }
}

void Game::resetGame()
{
    _killCount = 0;
    _enemySpeed = ENEMY_INITIAL_SPEED;
    _enemyHandler.reset();
}

void Game::gameLoop(float dt)
{
    _player.update(dt, _window);
    if (!_enemyHandler.update(dt) ||
        !handleCollisions())
    {
        _state = END;
    }
}


bool Game::handleCollisions()
{
    auto& enemies{_enemyHandler.getEnemies()};

    size_t i = 0;
    while (i < enemies.size())
    {
        if (collides(enemies[i]->getShape(), _player.getShape()))
            return false;
        if (checkEnemyBeenShot(*enemies[i]))
        {
            enemyType type{enemies[i]->getType()};
            _killCount += _enemyHandler.handlePowerup(type) + 1;
            if (type == BLACK_POWERUP)
                return true;
            if (type == RED_POWERUP)
                _player.activateRedPowerUp();
            safeRemoveFromVector(enemies, i);
            continue;
        }
        i++;
    }
    return true;
}

bool Game::checkEnemyBeenShot(Enemy& enemy)
{
    auto& projectiles{_player.getProjectiles()};

    for(size_t i = 0; i < projectiles.size(); i++)
    {
        if (collides(enemy.getShape(), projectiles[i]->getShape()))
        {
            safeRemoveFromVector(projectiles, i);
            return true;
        }
    }
    return false;
}

void Game::displayTexts(sf::RenderWindow &window)
{
    sf::Text text(_font);

    if (_state == START)
    {
        text.setString("How many aliens can you kill before\nthey reach your base?\n\nArrow keys to move, and space "
                          "to shoot!\nPress enter to begin...");
        text.setPosition({100.0f, 100.0f});
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(36);
    }
    else if (_state == GAME)
    {
        text.setString("Kill count: " + std::to_string(_killCount));
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition({10.0f, SCREEN_HEIGHT - 40.0f});
        
    }
    else if (_state == END)
    {
        text.setString("Final score: " + std::to_string(_killCount) + "\nPlay again? Press enter.");
        text.setPosition({100.0f, 100.0f});
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(36);
    }
    window.draw(text);
    
}

void Game::render()
{
    _window.clear(sf::Color::Black);
    displayTexts(_window);
    if (_state == GAME)
    {
        _player.draw(_window);
        _enemyHandler.draw(_window);
    }
    _window.display();
}