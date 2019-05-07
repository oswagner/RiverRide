/*
 *  PauseState.cpp
 *  Normal "play" state
 *
 *  Created by Marcelo Cohen on 08/13.
 *  Copyright 2013 PUCRS. All rights reserved.
 *
 */

#include <iostream>
#include <cmath>
#include "Game.h"
#include "PauseState.h"
#include "InputManager.h"

PauseState PauseState::m_PauseState;

using namespace std;

void PauseState::init()
{
    msg.load("data/img/paused.png");
	msg.setPosition(250,250);

	cout << "PauseState: Init" << endl;
}

void PauseState::cleanup()
{
	cout << "PauseState: Clean" << endl;
}

void PauseState::pause()
{
	cout << "PauseState: Paused" << endl;
}

void PauseState::resume()
{
	cout << "PauseState: Resumed" << endl;
}

void PauseState::handleEvents(cgf::Game* game)
{
    sf::Event event;

    while (screen->pollEvent(event))
    {
        if(event.key.code == sf::Keyboard::R)
            game->popState();
    }
}

void PauseState::update(cgf::Game* game)
{
}

void PauseState::draw(cgf::Game* game)
{
    screen = game->getScreen();
    screen->draw(msg);
}
