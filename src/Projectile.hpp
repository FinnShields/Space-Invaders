#pragma once

#include <SFML/Graphics.hpp>

class Projectile
{
  public:
    Projectile(sf::Vector2f position, float speed = 600.0f);
    ~Projectile() {};

    void update(float dt);
    void draw(sf::RenderWindow &window);
    bool isOffScreen(const sf::RenderWindow &window) const;
    sf::RectangleShape &getShape()
    {
        return _shape;
    };

  private:
    sf::RectangleShape _shape;
    float _speed;
};