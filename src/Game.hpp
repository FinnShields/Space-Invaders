#pragma once

#include <SFML/Graphics.hpp>

#include "Enemy.hpp"
#include "InputHandler.hpp"
#include "Player.hpp"
#include "constants.hpp"

class Game
{

  public:
    Game(sf::RenderWindow &window, Player &player, InputHandler &inputHandler);
    ~Game() {};

    void initEnemies();
    void run();
    void processEvents();
    void update();
    void render();
    Player &getPlayer()
    {
        return _player;
    };
    bool collides(sf::RectangleShape shape1, sf::RectangleShape shape2);
    void handleCollisions(Enemy* enemy);
    void addNewEnemies();
    void handleEnemyMovement(float dt);
    void handleEnemySpeed(float dt);
    void handlePowerups(enemyType type);
    void activatePurplePowerup();
    void displayTexts(sf::RenderWindow &window);

  private:
    sf::RenderWindow &_window;
    Player &_player;
    InputHandler &_inputHandler;
    std::vector<Enemy*> _enemies;
    float _enemyDirection = 1.0f;
    float _enemyMoveDown = ENEMY_MOVE_DOWN;
    float _enemySpeed = ENEMY_INITIAL_SPEED;
    float _enemySpeedTimer = 0.0f;
    float _bluePowerupTime = 0.0f;
    int _killCount = 0;
    sf::Clock _clock;
    sf::Font _font;
};