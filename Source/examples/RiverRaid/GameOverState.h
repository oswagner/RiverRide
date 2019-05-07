#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include "GameState.h"
#include "Sprite.h"
#include "InputManager.h"

class GameOverState : public cgf::GameState
{
    public:

    void init();
    void cleanup();

    void pause();
    void resume();

    void handleEvents(cgf::Game* game);
    void update(cgf::Game* game);
    void draw(cgf::Game* game);

    // Implement Singleton Pattern
    static GameOverState* instance()
    {
        return &m_GameOverState;
    }

    protected:

    GameOverState() {}

    private:

    static GameOverState m_GameOverState;

    sf::Font font;
    sf::Text gameOverText;
    sf::RenderWindow* screen;
};

#endif
