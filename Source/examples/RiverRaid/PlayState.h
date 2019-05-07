/*
 *  PlayState.h
 *  Normal "play" state
 *
 *  Created by Gustavo de Lima, Matheus Alano and Matheus Britzke
 *  Copyright 2018 PUCRS. All rights reserved.
 *
 */

#ifndef PLAY_STATE_H_
#define PLAY_STATE_H_

#include "GameState.h"
#include "Sprite.h"
#include "InputManager.h"
#include <vector>
#include <tmx/MapLoader.h>

class PlayState : public cgf::GameState
{
    public:

    void init();
    void cleanup();

    void pause();
    void resume();

    void handleEvents(cgf::Game* game);
    void update(cgf::Game* game);
    void draw(cgf::Game* game);
    int randomNumber(int min, int max);
    cgf::Sprite createEnemy();
    cgf::Sprite createFuelTank();

    int points;

    // Implement Singleton Pattern
    static PlayState* instance()
    {
        return &m_PlayState;
    }

    protected:

    PlayState() {}

    private:

    static PlayState m_PlayState;

    int dirx;
    tmx::MapLoader* map;
    cgf::Sprite playSprite1;
    std::vector <cgf::Sprite> bullets;
    std::vector <cgf::Sprite> enemies;
    std::vector <cgf::Sprite> fuelTanks;
    sf::RectangleShape fuelBackground;
    sf::RectangleShape fuel;
    float fuelLeft;
    sf::Font font;
    sf::Text fuelText;
    sf::Text lifesText;
    sf::Text pointsText;
    int lifesLeft;
    float lastEnemyPos;
    sf::RenderWindow* screen;
    cgf::InputManager* im;

    sf::SoundBuffer gunSoundBuffer;
    sf::Sound gunSound;

    sf::SoundBuffer fuelSoundBuffer;
    sf::Sound fuelSound;

    sf::SoundBuffer enemySoundBuffer;
    sf::Sound enemySound;
};

#endif
