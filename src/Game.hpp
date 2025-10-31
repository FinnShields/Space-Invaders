#pragma once

#include <SFML/Graphics.hpp>

#include "Enemy.hpp"
#include "EnemyHandler.hpp"
#include "InputHandler.hpp"
#include "Player.hpp"
#include "constants.hpp"
#include "utils.hpp"

enum State
{
  START,
  GAME,
  END
};

class Game
{
  public:
    Game(sf::RenderWindow &window, Player &player, InputHandler &inputHandler, EnemyHandler& enemyHandler);
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
    bool handleCollisions();
    int handleCollisions2(size_t enemyIndex);
    void addNewEnemies();
    void handleEnemyMovement(float dt);
    void handleEnemySpeed(float dt);
    void handlePowerups(enemyType type);
    void activatePurplePowerup();
    void activateBlackPowerup();
    void displayTexts(sf::RenderWindow &window);
    void gameLoop(float dt);
    void resetGame();
    void setState(State state) {
      _state = state;
    }

  private:
    sf::RenderWindow &_window;
    Player &_player;
    const InputHandler &_inputHandler;
    EnemyHandler& _enemyHandler;

    bool checkEnemyBeenShot(Enemy& enemy);
    void updateKillCounter(std::vector<Enemy*> enemies);

    std::vector<Enemy *> _enemies;
    float _enemyDirection = 1.0f;
    float _enemyMoveDown = ENEMY_MOVE_DOWN;
    float _enemySpeed = ENEMY_INITIAL_SPEED;
    float _enemySpeedTimer = 0.0f;
    float _bluePowerupTime = 0.0f;
    int _killCount = 0;
    sf::Clock _clock;
    sf::Font _font;
    State _state = START;
};