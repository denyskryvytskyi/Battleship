#ifndef _GAME_MANAGER_H
#define _GAME_MANAGER_H

#include "Player.h"

enum class EGameState
{
    PlacingShips,
    Battle,
    Gameover,
};

enum class EGameMode
{
    OnePlayer,
    TwoPlayer,
    ExitGame,
};

class GameManager {

    typedef std::vector<PlayerPtr> PlayerVector;

private:
    sf::RenderWindow mMainWindow;
    //
    EGameState mGameState;
    EGameMode mGameMode;
    //
    PlayerVector mPlayers;
    //
    int mSecondPlayerPosOffset;
    //
    sf::Text mGameOverText;

private:
    bool Initialize();
    //
    bool TryPlacing(sf::Vector2i pos);
    bool TryFire(sf::Vector2i pos);
    //
    void HideShips() const;
    void Update();
    void ProcessEvents();
    //
    void Render();
    void RenderPlayer(PlayerPtr player);
    void RenderGameOver();

public:
    GameManager(const int mode);
    ~GameManager();
    //
    void Start();
};

#endif // !_GAME_MANAGER_H
