#include <iostream>
#include <cmath>
#include "Game.h"
#include "GameOverState.h"
#include "PlayState.h"
#include "InputManager.h"

GameOverState GameOverState::m_GameOverState;

using namespace std;

void GameOverState::init()
{

    if (!font.loadFromFile("data/fonts/arial.ttf")) {
        cout << "Cannot load arial.ttf font!" << endl;
        exit(1);
    }
    gameOverText.setFont(font);
    PlayState points = *PlayState::instance();
    string gameover = "Game Over \n Score: " + std::to_string(points.points) + " \n Press R to reload";
    gameOverText.setString(gameover);
    gameOverText.setCharacterSize(34);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(275, 200);
    gameOverText.setStyle(sf::Text::Bold);

	//PlayState::instance().

	cout << "GameOverState: Init" << endl;
}

void GameOverState::cleanup()
{
	cout << "GameOverState: Clean" << endl;
}

void GameOverState::pause()
{
	cout << "GameOverState: Paused" << endl;
}

void GameOverState::resume()
{
	cout << "GameOverState: Resumed" << endl;
}

void GameOverState::handleEvents(cgf::Game* game)
{
    sf::Event event;

    while (screen->pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            game->quit();
        if(event.key.code == sf::Keyboard::R)
            game->changeState(PlayState::instance());


    }

    if(event.key.code == sf::Keyboard::Escape)
            game->quit();
}

void GameOverState::update(cgf::Game* game)
{
}

void GameOverState::draw(cgf::Game* game)
{
    screen = game->getScreen();
    screen->draw(gameOverText);
}
