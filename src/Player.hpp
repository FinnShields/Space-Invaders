#pragma once
#include "Projectile.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include "constants.hpp"

class Player
{
  public:
    Player(float x, float y);
    ~Player() {};

    void update(float dt, sf::RenderWindow &window);
    void draw(sf::RenderWindow &window);

    sf::RectangleShape &getShape()
    {
        return _shape;
    };
    void movePlayerRight(bool b);
    void movePlayerLeft(bool b);
    std::vector<Projectile*> &getProjectiles()
    {
        return _projectiles;
    };
    void shoot();
    void activateRedPowerUp();

  private:
    sf::RectangleShape _shape;
    float _speed = 300.0f;

    std::vector<Projectile*> _projectiles;
    float _shootCooldown = SHOT_COOLDOWN;
    float _timeSinceLastShot = 0.0f;
    bool _moveRight = false;
    bool _moveLeft = false;
    float _redPowerUpTime = 0.0f;
};
