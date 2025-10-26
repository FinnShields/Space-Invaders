#include "Enemy.hpp"

Enemy::Enemy(float x, float y)
{
	_shape.setSize(sf::Vector2f(40.0f, 20.0f));
	_type = getRandomType();
	sf::Color colour = _type == RED_POWERUP ? sf::Color::Red 
								: _type == BLUE_POWERUP ? sf::Color::Blue 
								: _type == PURPLE_POWERUP ? sf::Color::Magenta
								: sf::Color::White;
	_shape.setFillColor(colour);
	_shape.setPosition({x, y});
}

enemyType Enemy::getRandomType()
{
	if ((std::rand() % 100) <= 2)
		return RED_POWERUP;
	if ((std::rand() % 100) <= 2)
		return BLUE_POWERUP;
	if ((std::rand() % 100) <= 2)
		return PURPLE_POWERUP;
	return NORMAL;
}

void Enemy::draw(sf::RenderWindow& window) 
{
	window.draw(_shape);
}