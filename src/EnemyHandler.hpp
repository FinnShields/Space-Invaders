#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "constants.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "utils.hpp"

class EnemyHandler
{
    public:
        //std::vector<Enemy*> getEnemies();
        EnemyHandler() = delete;
        EnemyHandler(Player& player);
        ~EnemyHandler(){}
        bool update(float dt);
        void reset();
        void draw(sf::RenderWindow& target);

        size_t handlePowerup(enemyType type);
        std::vector<Enemy*>& getEnemies(){ return _enemies; }
    
    private:
        std::vector<Enemy*>         _enemies;
        std::vector<Projectile*>&   _projectiles;
        float _enemySpeed = ENEMY_INITIAL_SPEED;
        float _enemyDirection = 1.0f;
        float _enemySpeedTimer = 0.0f;
        float _bluePowerupTime = 0.0f;


        void initEnemies();
        void handleEnemySpeed(float dt);
        bool handleEnemyMovement(float dt);
        void addNewEnemies();
        size_t activateBluePowerup();
        size_t activateBlackPowerup();
        size_t activatePurplePowerup();
};