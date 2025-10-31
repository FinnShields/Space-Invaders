#include "EnemyHandler.hpp"

EnemyHandler::EnemyHandler(Player& player) : _projectiles(player.getProjectiles())
{
    initEnemies();
}

bool EnemyHandler::update(float dt)
{
    if (!handleEnemyMovement(dt))
        return false;
    return true;
}
void EnemyHandler::reset()
{
    _enemies.clear();
    initEnemies();
}

void EnemyHandler::draw(sf::RenderWindow& target)
{
    for (auto enemy : _enemies)
        enemy->draw(target);
}

size_t EnemyHandler::handlePowerup(enemyType type)
{
    return type == BLUE_POWERUP ? activateBluePowerup()
        : type == PURPLE_POWERUP ? activatePurplePowerup()
        : type == BLACK_POWERUP ? activateBlackPowerup()
        : 0;
}

void EnemyHandler::initEnemies()
{
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 10; ++col)
        {
            float x = 60 + col * 60;
            float y = 10 + row * 40;
            Enemy *newEnemy = new Enemy(x, y);
            _enemies.push_back(newEnemy);
        }
    }
}

void EnemyHandler::handleEnemySpeed(float dt)
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

bool EnemyHandler::handleEnemyMovement(float dt)
{
    handleEnemySpeed(dt);
    addNewEnemies();

    bool changeDirection{false};
    int enemyCount{0};
    for (auto enemy : _enemies)
    {
        auto& shape{enemy->getShape()};
        float moveX{_enemyDirection * _enemySpeed * dt};
        if (_bluePowerupTime > 0.0f)
            moveX = 0;
        shape.move({moveX, 0});

        sf::Vector2f pos{shape.getPosition()};
        if (pos.x <= 0 || pos.x >= (SCREEN_WIDTH - shape.getSize().x))
        {
            changeDirection = true;
        }

        //change game state if enemy touches the ground
        if (pos.y >= SCREEN_HEIGHT - shape.getSize().y)
            return false;
        enemyCount++;
    }
    if (changeDirection)
    {
        _enemyDirection *= -1;
        for (auto enemy : _enemies)
            enemy->getShape().move({_enemyDirection * _enemySpeed * dt, ENEMY_MOVE_DOWN});
    }
    return true;
}

void EnemyHandler::addNewEnemies()
{
    float lowestEnemyY = __FLT_MAX__;
    for (auto enemy : _enemies)
    {
        lowestEnemyY =
            (enemy->getShape().getPosition().y < lowestEnemyY) ? (enemy->getShape().getPosition().y) : lowestEnemyY;
    }
    if (lowestEnemyY <= 10.0f)
        return;
    float enemyX = (_enemyDirection == 1) ? 0 : 220;
    if (_enemies.size() == 0)
        enemyX = 0;
    for (int col = 0; col < 10; ++col)
    {
        float x = enemyX + col * 60;
        float y = -10;
        Enemy *newEnemy = new Enemy(x, y);
        _enemies.push_back(newEnemy);
    }
}

size_t EnemyHandler::activateBluePowerup()
{
    _bluePowerupTime = 5.0f;
    return 0;
}

size_t EnemyHandler::activateBlackPowerup()
{
    size_t enemyCount = _enemies.size();
    _enemies.clear();
    return enemyCount;
}

size_t EnemyHandler::activatePurplePowerup()
{
    size_t i = 0;
    size_t enemyCount = 0;
    while (i < _enemies.size())
    {
        if (_enemies[i]->getType() != NORMAL)
        {
            i++;
            continue;
        }
        if (std::rand() % 100 < 20)
        {
            safeRemoveFromVector(_enemies, i);
            enemyCount++;
            continue;
        }
        i++;
    }
    return enemyCount;
}