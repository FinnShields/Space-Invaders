#include "Projectile.hpp"

Projectile::Projectile(sf::Vector2f position, float speed)
    : _speed(speed)
{
    _shape.setSize({5.0f, 15.0f});
    _shape.setFillColor(sf::Color::White);
    _shape.setPosition(position);
}

void Projectile::update(float dt)
{
    _shape.move({0.0f, -_speed * dt});
}

void Projectile::draw(sf::RenderWindow& window)
{
    window.draw(_shape);
}

bool Projectile::isOffScreen(const sf::RenderWindow& window) const
{
    return _shape.getPosition().y + _shape.getSize().y < 0;
} 
