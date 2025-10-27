#include "Game.hpp"
#include <algorithm>
#include <iostream>

Game::Game(sf::RenderWindow &window, Player &player, InputHandler &inputHandler)
    : _window(window), _player(player), _inputHandler(inputHandler)
{
    initEnemies();
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

void Game::initEnemies()
{
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 10; ++col)
        {
            float x = 60 + col * 60;
            float y = 0 + row * 40;
            Enemy *newEnemy = new Enemy(x, y);
            _enemies.push_back(newEnemy);
        }
    }
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
    _enemies.clear();
    _killCount = 0;
    initEnemies();
    _enemySpeed = ENEMY_INITIAL_SPEED;
}

void Game::gameLoop(float dt)
{
    _player.update(dt, _window);
    handleEnemySpeed(dt);
    handleEnemyMovement(dt);
    addNewEnemies();
}

void Game::handleEnemySpeed(float dt)
{
    _enemySpeedTimer += dt;
    if (_enemySpeedTimer >= 2.5f)
    {
        _enemySpeed += 6;
        _enemySpeedTimer = 0.0f;
    }

    if (_bluePowerupTime > 0)
        _bluePowerupTime -= dt;
}

void Game::handleEnemyMovement(float dt)
{
    bool changeDirection = false;
    size_t i = 0;
    while (i < _enemies.size())
    {
        sf::RectangleShape &enemyShape = _enemies[i]->getShape();
        float moveX = _enemyDirection * _enemySpeed * dt;
        if (_bluePowerupTime > 0.0f)
            moveX = 0;
        enemyShape.move({moveX, 0});
        if (enemyShape.getPosition().x <= 0 ||
            enemyShape.getPosition().x + enemyShape.getSize().x >= _window.getSize().x)
        {
            changeDirection = true;
        }
        if (enemyShape.getPosition().y >= (SCREEN_HEIGHT - (enemyShape.getSize().y / 2)))
            _state = END;
        if (handleCollisions(i) == 0)
            i++;
    }
    static int count = 0;
    if (changeDirection)
    {
        _enemyDirection *= -1;
        for (auto enemy : _enemies)
        {
            sf::RectangleShape &enemyShape = enemy->getShape();
            enemyShape.move({_enemyDirection * _enemySpeed * dt, _enemyMoveDown});
        }
    }
}

int Game::handleCollisions(size_t enemyIndex)
{
    if (collides(_enemies[enemyIndex]->getShape(), _player.getShape()))
        _state = END;

    auto &projectiles = _player.getProjectiles();
    size_t i = 0;
    while (i < projectiles.size())
    {
        if (collides(_enemies[enemyIndex]->getShape(), projectiles[i]->getShape()))
        {
            handlePowerups(_enemies[enemyIndex]->getType());
            if (_enemies.size() == 0)
                return 0;
            std::swap(projectiles[i], projectiles.back());
            projectiles.pop_back();
            std::swap(_enemies[enemyIndex], _enemies.back());
            _enemies.pop_back();
            _killCount++;
            return 1;
        }
        i++;
    }
    return 0;
}

void Game::handlePowerups(enemyType type)
{
    if (type == NORMAL)
        return;
    else if (type == RED_POWERUP)
        _player.activateRedPowerUp();
    else if (type == BLUE_POWERUP)
        _bluePowerupTime = 3.0f;
    else if (type == PURPLE_POWERUP)
        activatePurplePowerup();
    else if (type == BLACK_POWERUP)
        activateBlackPowerup();
}

void Game::activatePurplePowerup()
{
    size_t i = 0;
    while (i < _enemies.size())
    {
        if (_enemies[i]->getType() != NORMAL)
        {
            i++;
            continue;
        }
        if (std::rand() % 100 < 20)
        {
            std::swap(_enemies[i], _enemies.back());
            _enemies.pop_back();
            continue;
            _killCount++;
        }
        i++;
    }
}

void Game::activateBlackPowerup()
{
    _killCount += _enemies.size();
    _enemies.clear();
}

void Game::addNewEnemies()
{
    float lowestEnemyX = __FLT_MAX__;
    float lowestEnemyY = __FLT_MAX__;
    for (auto enemy : _enemies)
    {
        lowestEnemyX =
            (enemy->getShape().getPosition().x < lowestEnemyX) ? (enemy->getShape().getPosition().x) : lowestEnemyX;
        lowestEnemyY =
            (enemy->getShape().getPosition().y < lowestEnemyY) ? (enemy->getShape().getPosition().y) : lowestEnemyY;
    }
    if (_enemies.size() == 0)
        lowestEnemyX = 20.0f;
    if (lowestEnemyY <= 20.0f or lowestEnemyX > 60.0f or lowestEnemyX < 0.0f)
        return;
    for (int col = 0; col < 10; ++col)
    {
        float x = lowestEnemyX + col * 60;
        float y = 0;
        Enemy *newEnemy = new Enemy(x, y);
        _enemies.push_back(newEnemy);
    }
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
        for (auto enemy : _enemies)
            enemy->draw(_window);
    }
    _window.display();
}