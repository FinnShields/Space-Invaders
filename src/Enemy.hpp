#pragma once

#include <SFML/Graphics.hpp>

enum enemyType
{
    NORMAL,
    RED_POWERUP,
    BLUE_POWERUP,
    PURPLE_POWERUP,
    BLACK_POWERUP
};

class Enemy
{

  public:
    Enemy(float x, float y);
    ~Enemy() {};

    void draw(sf::RenderWindow &window);

    sf::RectangleShape &getShape()
    {
        return _shape;
    };
    enemyType getType()
    {
        return _type;
    };

  private:
    sf::RectangleShape _shape;
    float _speed = 50.0f;
    enemyType _type;
    enemyType getRandomType();
};