/*
 *  PlayState.cpp
 *  Normal "play" state
 *
 *  Created by Gustavo de Lima, Matheus Alano and Matheus Britzke
 *  Copyright 2018 PUCRS. All rights reserved.
 *
 */

#include <iostream>
#include <cmath>
#include "Game.h"
#include "PlayState.h"
#include "PauseState.h"
#include "GameOverState.h"
#include "InputManager.h"

PlayState PlayState::m_PlayState;

using namespace std;

void PlayState::init()
{
    srand( time(NULL) );

    map = new tmx::MapLoader("data/maps");
    map->Load("mapBackup.tmx");

    playSprite1.loadXML("data/img/player.xml");
    playSprite1.setPosition(369, 430);
    playSprite1.setFrameRange(0,15);
    playSprite1.setAnimRate(30);
    playSprite1.setLooped(true);
    playSprite1.play();

    fuelLeft = 1.0;
    sf::Vector2f fuelSize(300 * fuelLeft, 50);
    fuel.setSize(fuelSize);
    fuel.setPosition(250, 530);
    fuel.setFillColor(sf::Color::Green);

    sf::Vector2f fuelBGSize(320, 70);
    fuelBackground.setSize(fuelBGSize);
    fuelBackground.setPosition(240, 520);
    fuelBackground.setFillColor(sf::Color::Red);

     if (!font.loadFromFile("data/fonts/arial.ttf")) {
        cout << "Cannot load arial.ttf font!" << endl;
        exit(1);
    }
    fuelText.setFont(font);
    fuelText.setString("Fuel");
    fuelText.setCharacterSize(34);
    fuelText.setFillColor(sf::Color::Black);
    fuelText.setPosition(375, 533);
    fuelText.setStyle(sf::Text::Bold);

    lifesLeft = 3;

    lifesText.setFont(font);
    lifesText.setString(std::to_string(lifesLeft));
    lifesText.setCharacterSize(50);
    lifesText.setFillColor(sf::Color::White);
    lifesText.setPosition(190, 525);
    lifesText.setStyle(sf::Text::Bold);

    points = 0;

    pointsText.setFont(font);
    pointsText.setString(std::to_string(points));
    pointsText.setCharacterSize(50);
    pointsText.setFillColor(sf::Color::White);
    pointsText.setPosition(620, 525);
    pointsText.setStyle(sf::Text::Bold);

    dirx = 0;
    lastEnemyPos = 400;

    gunSoundBuffer.loadFromFile("data/audio/gun.wav");
    gunSound.setBuffer(gunSoundBuffer);

    fuelSoundBuffer.loadFromFile("data/audio/superjump.wav");
    fuelSound.setBuffer(fuelSoundBuffer);

    enemySoundBuffer.loadFromFile("data/audio/enemy.wav");
    enemySound.setBuffer(enemySoundBuffer);

    im = cgf::InputManager::instance();

    im->addKeyInput("left", sf::Keyboard::Left);
    im->addKeyInput("right", sf::Keyboard::Right);
    im->addKeyInput("space", sf::Keyboard::Space);
    im->addKeyInput("quit", sf::Keyboard::Escape);
    im->addKeyInput("stats", sf::Keyboard::S);

    cout << "PlayState: Init" << endl;
}

void PlayState::cleanup()
{
    delete map;
    cout << "PlayState: Clean" << endl;
}

void PlayState::pause()
{
    cout << "PlayState: Paused" << endl;
}

void PlayState::resume()
{
    cout << "PlayState: Resumed" << endl;
}

void PlayState::handleEvents(cgf::Game* game)
{
    screen = game->getScreen();
    sf::Event event;

    while (screen->pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            game->quit();

        if(event.key.code == sf::Keyboard::P)
            game->pushState(PauseState::instance());

        if(event.type == sf::Event::KeyPressed && im->testEvent("space")) {
            cgf::Sprite bulletSprite;
            bulletSprite.load("data/img/bullet.png");
            bulletSprite.setScale(0.5, 0.5);
            float xPos = playSprite1.getPosition().x + (playSprite1.getSize().x / 2) - (bulletSprite.getSize().x / 2) * 0.5;
            bulletSprite.setPosition(xPos, playSprite1.getPosition().y);
            bulletSprite.setYspeed(-300.0);
            bullets.push_back(bulletSprite);

            if(gunSound.getStatus() == sf::Sound::Playing) {
                gunSound.stop();
            }
            gunSound.play();
        }
    }

    dirx = 0;

    if(im->testEvent("left")) {
        if(playSprite1.getPosition().x > 0) {
            dirx--;
        }
        if (!playSprite1.getMirror()) playSprite1.setMirror(true);
    }


    if(im->testEvent("right")) {
        if((playSprite1.getPosition().x + playSprite1.getSize().x) < screen->getSize().x) {
            dirx++;
        }
        if (playSprite1.getMirror()) playSprite1.setMirror(false);
    }

    if(im->testEvent("quit"))
        game->quit();

    if(im->testEvent("stats"))
        game->toggleStats();

    playSprite1.setXspeed(200*dirx);
}

void PlayState::update(cgf::Game* game)
{
    playSprite1.update(game->getUpdateInterval());

    if (fuelLeft <= 0) {
        fuelLeft = 1.0;
        lifesLeft--;
        if (lifesLeft < 0) game->changeState(GameOverState::instance());
        else lifesText.setString(std::to_string(lifesLeft));
    } else {
        fuelLeft -= 0.001;
    }
    sf::Vector2f fuelSize(300 * fuelLeft, 50);
    fuel.setSize(fuelSize);

    if ((playSprite1.getPosition().x <= 128) || (playSprite1.getPosition().x + playSprite1.getSize().x >= 680)) {
        lifesLeft--;
        playSprite1.setPosition(369, 430);
        enemies.clear();
        fuelTanks.clear();
        if (lifesLeft < 0) game->changeState(GameOverState::instance());
        else lifesText.setString(std::to_string(lifesLeft));
    }

    for(int i = 0; i < bullets.size(); i++) {
        bool isErased = false;
        for(int j = 0; j < enemies.size(); j++) {
            if (bullets[i].bboxCollision(enemies[j])) {
                enemySound.play();
                bullets.erase(bullets.begin() + i);
                enemies.erase(enemies.begin() + j);
                isErased = true;
                points = points + 30;
                break;
            }
        }
        if (isErased) continue;

        for(int k = 0; k < fuelTanks.size(); k++) {
            if (bullets[i].bboxCollision(fuelTanks[k])) {
                bullets.erase(bullets.begin() + i);
                fuelTanks.erase(fuelTanks.begin() + k);
                isErased = true;
                points = points + 80;
                break;
            }
        }
        if (isErased) continue;

        if (bullets[i].getPosition().y <= 0) {
            bullets.erase(bullets.begin() + i);
        } else {
            bullets[i].update(game->getUpdateInterval());
        }
    }

    for(int i = 0; i < enemies.size(); i++) {
        if (playSprite1.circleCollision(enemies[i])) {
            enemies.erase(enemies.begin() + i);
            lifesLeft--;
            if (lifesLeft < 0) game->changeState(GameOverState::instance());
            else lifesText.setString(std::to_string(lifesLeft));
        } else if (enemies[i].getPosition().y + enemies[i].getSize().y >= 600) {
            enemies.erase(enemies.begin() + i);
        } else if (enemies[i].getPosition().x <= 128) {
            enemies[i].setMirror(false);
            enemies[i].setXspeed(100.0);
        } else if (enemies[i].getPosition().x + enemies[i].getSize().x >= 680) {
            enemies[i].setMirror(true);
            enemies[i].setXspeed(-100.0);
        }
        enemies[i].update(game->getUpdateInterval());
    }

    for(int i = 0; i < fuelTanks.size(); i++) {
        if(playSprite1.circleCollision(fuelTanks[i])) {
            fuelSound.play();
            fuelTanks.erase(fuelTanks.begin() + i);
            fuelLeft = min(fuelLeft + 0.2, 1.0);
            sf::Vector2f fuelSize(300 * fuelLeft, 50);
            fuel.setSize(fuelSize);
        } else if (fuelTanks[i].getPosition().y + fuelTanks[i].getSize().y >= 600) {
            fuelTanks.erase(fuelTanks.begin() + i);
        }
        fuelTanks[i].update(game->getUpdateInterval());
    }

    if (enemies.size() <= 3 && randomNumber(1, 500) <= 5) {
        enemies.push_back(createEnemy());
    }
    if (fuelTanks.size() <= 1 && randomNumber(1,500) <= 3) {
        fuelTanks.push_back(createFuelTank());
    }
    pointsText.setString(std::to_string(points));
}

void PlayState::draw(cgf::Game* game)
{
    screen = game->getScreen();
    map->Draw(*screen, 0);
    screen->draw(playSprite1);

    for(int i = 0; i < bullets.size(); i++) {
        screen->draw(bullets[i]);
    }
    for(int i = 0; i < enemies.size(); i++) {
        screen->draw(enemies[i]);
    }
    for(int i = 0; i < fuelTanks.size(); i++) {
        screen->draw(fuelTanks[i]);
    }

    screen->draw(fuelBackground);
    screen->draw(fuel);
    screen->draw(fuelText);
    screen->draw(lifesText);
    screen->draw(pointsText);
}

int PlayState::randomNumber(int min, int max) {
    return min + rand() % (max - min + 1);
}

cgf::Sprite PlayState::createEnemy() {
    cgf::Sprite enemySprite;
    string enemyFilePath = "data/img/enemy" + std::to_string(randomNumber(1, 3)) + ".png";
    enemySprite.load(enemyFilePath.c_str());
    float xPos;
    if (lastEnemyPos > 375) xPos = randomNumber(150, 375);
    else xPos = randomNumber(375, 600);
    lastEnemyPos = xPos;
    enemySprite.setPosition(xPos, 5);
    if (points > 800) {
        enemySprite.setYspeed(40.0);
        enemySprite.setXspeed(130.0);
    }
    else if (points > 200) {
        enemySprite.setYspeed(20.0);
        enemySprite.setXspeed(70.0);
    } else {
        enemySprite.setYspeed(30.0);
    }
    return enemySprite;
}

cgf::Sprite PlayState::createFuelTank() {
    cgf::Sprite fuelSprite;
    fuelSprite.load("data/img/fuel.png");
    float xPos = randomNumber(150, 600);
    fuelSprite.setPosition(xPos, 5);
    fuelSprite.setYspeed(40.0);
    return fuelSprite;
}
