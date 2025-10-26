#include "Player.hpp"

Player::Player(float x, float y)
{
    _shape.setSize({50.0f, 20.0f});
    _shape.setFillColor(sf::Color::White);
    _shape.setPosition({x, y});
}

void Player::update(float dt, sf::RenderWindow &window)
{
    if (_moveLeft)
        _shape.move({-_speed * dt, 0});
    if (_moveRight)
        _shape.move({_speed * dt, 0});
    if (_redPowerUpTime > 0)
    {
        _redPowerUpTime -= dt;
        _shootCooldown = 0.1f;
    }
    if (_redPowerUpTime <= 0)
    {
        _redPowerUpTime = 0;
        _shootCooldown = SHOT_COOLDOWN;
    }

    if (_shape.getPosition().x < 0)
        _shape.setPosition({0, _shape.getPosition().y});
    if (_shape.getPosition().x + _shape.getSize().x > window.getSize().x)
        _shape.setPosition({window.getSize().x - _shape.getSize().x, _shape.getPosition().y});

    _timeSinceLastShot += dt;

    for (auto p : _projectiles)
    {
        p->update(dt);
        if (p->isOffScreen(window))
            _projectiles.erase(std::find(_projectiles.begin(), _projectiles.end(), p));
    }
}

void Player::activateRedPowerUp()
{
    _redPowerUpTime = 5.0f;
}

void Player::movePlayerRight(bool b)
{
    if (b)
        _moveRight = true;
    else
        _moveRight = false;
}

void Player::movePlayerLeft(bool b)
{
    if (b)
        _moveLeft = true;
    else
        _moveLeft = false;
}

void Player::draw(sf::RenderWindow &window)
{
    window.draw(_shape);
    for (auto &p : _projectiles)
        p->draw(window);
}

void Player::shoot()
{
    if (_timeSinceLastShot < _shootCooldown)
        return;
    sf::Vector2f pos = _shape.getPosition();
    pos.x += _shape.getSize().x / 2.0f - 2.5f;
    pos.y -= 10.0f;
    Projectile* newProjectile = new Projectile(pos);
    _projectiles.emplace_back(newProjectile);
    _timeSinceLastShot = 0.0f;
}
