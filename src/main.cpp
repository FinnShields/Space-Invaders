// SpaceInvaders.cpp

#include <SFML/Graphics.hpp>
#include <vector>

#include "Game.hpp"
#include "constants.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "spaceInvadersWindow");
    Player player(SCREEN_WIDTH / 2, PLAYER_HEIGHT);
    InputHandler inputHandler(window, player);
    Game game(window, player, inputHandler);

    game.run();

    return 0;
}
