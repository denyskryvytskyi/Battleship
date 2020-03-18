#include "GameManager.h"
#include "PlayerFactory.h"
#include "Config.h"
#include "CommonTypes.h"

GameManager::GameManager(const int mode) :
    mMainWindow(sf::VideoMode(cfg::mode_width, cfg::mode_height), "Battleship", sf::Style::Titlebar | sf::Style::Close),
    mSecondPlayerPosOffset(cfg::mode_width / 2),
    mGameMode((EGameMode)mode),
    mGameState(EGameState::PlacingShips),
    mIsShipsHidden(false)
{
    Initialize();
}

GameManager::~GameManager()
{
    for (auto player : mPlayers)
    {
        delete player;
    }
    mPlayers.clear();
}

bool GameManager::Initialize()
{
    sf::FloatRect playerField(0, 0, cfg::mode_width / 2, cfg::mode_height);

    mPlayers.push_back(PlayerFactory::Create(mPlayers.size(), sf::FloatRect(0, 0, 0.5f, 1), playerField, "Player 1", EPlayerState::Placing));

    if (mGameMode == EGameMode::TwoPlayer)
    {
        PlayerPtr player2 = PlayerFactory::Create(mPlayers.size(), sf::FloatRect(0.5f, 0, 0.5f, 1), playerField, "Player 2", EPlayerState::Placing); // create RealPlayer
        mPlayers.push_back(player2);
    }
    //else if (mGameMode == EGameMode::OnePlayer) // isn't implemented yet
    //{}

    return true;
}

void GameManager::Start()
{
    while (mMainWindow.isOpen())
    {
        if (mGameState == EGameState::Gameover)
            break;

        ProcessEvents();
        Update();
        if (mGameState == EGameState::Gameover)
        {
            RenderGameOver();
            break;
        }
        Render();
    }
}

void GameManager::Update()
{
    bool playersReady = true;

    for (PlayerPtr player : mPlayers)
    {
        player->Update();
        if (player->GetCurrentState() == EPlayerState::Dead)
        {
            mPlayerLoseId = player->mId;
            mGameState = EGameState::Gameover;
            return;
        }
        if (player->GetCurrentState() != EPlayerState::Ready)
        {
            playersReady = false;
        }
    }

    if (playersReady)
    {
        mGameState = EGameState::Battle;
        mPlayers[0]->SetCurrentState(EPlayerState::Fire);
        mPlayers[1]->SetCurrentState(EPlayerState::Wait);
    }
}

void GameManager::ProcessEvents()
{
    sf::Event event;
    while (mMainWindow.pollEvent(event))
    {
        if (event.type == KeyCode_WinClosed || mGameMode == EGameMode::ExitGame)
            mMainWindow.close();

        if (event.type == KeyCode_MouseBtnPressed)
        {
            if (event.mouseButton.button == KeyCode_LeftMouseBtn)
            {
                sf::Vector2i pos = sf::Mouse::getPosition(mMainWindow);

                if (mGameState == EGameState::PlacingShips)
                {
                    TryPlacing(pos);
                }
                if (mGameState == EGameState::Battle)
                {
                    TryFire(pos);
                }
            }
        }

        // now implemented auto hiding
        /*if (event.mouseButton.button == KeyCode_H)
        {
            HideShips();
        }*/
    }
}

void GameManager::Render()
{
    mMainWindow.clear(color::Background);

    for (PlayerPtr player : mPlayers)
    {
        RenderPlayer(player);
    }

    mMainWindow.display();
}

void GameManager::RenderPlayer(PlayerPtr player)
{
    mMainWindow.setView(player->mCamera);

    for (int i = 0; i < cfg::field_rows; i++)
        for (int j = 0; j < cfg::field_rows; j++)
        {
            mMainWindow.draw(player->GetCell(i, j));
        }

    player->mName.setPosition(player->mCamera.getCenter().x - 25, 0);
    mMainWindow.draw(player->mName);
}

void GameManager::RenderGameOver()
{
    sf::RenderWindow gameOver(sf::VideoMode(250, 60), "Game over");

    while (gameOver.isOpen())
    {
        sf::Event event;
        while (gameOver.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                gameOver.close();
        }
        sf::Font font;
        font.loadFromFile("arial.ttf");
        sf::Text txt;
        txt.setString("Player %d lost" + mPlayerLoseId + 1);
        txt.setFont(font);
        txt.setFillColor(sf::Color::Red);

        txt.setCharacterSize(20);

        gameOver.clear();
        gameOver.draw(txt);
        gameOver.display();
    }
}

bool GameManager::TryPlacing(sf::Vector2i pos)
{
    for (PlayerPtr player : mPlayers)
    {
        if (player->GetCurrentState() == EPlayerState::Placing)
        {
            if (player->mId == 1)
            {
                pos.x -= mSecondPlayerPosOffset;
            }
            player->AddDeck(pos);
            return true;
        }
    }

    return false;
}

bool GameManager::TryFire(sf::Vector2i pos)
{
    for (PlayerPtr player : mPlayers)
    {
        if (player->GetCurrentState() == EPlayerState::Fire)
        {
            if (player->mId == 1)
            {
                pos.x -= mSecondPlayerPosOffset;
            }
            player->Fire(pos);
            if (player->GetCurrentState() == EPlayerState::Wait)
            {
                int nextPlayerID = player->mId == 0 ? 1 : 0;
                SetPlayerState(nextPlayerID, EPlayerState::Fire);
            }
            return true;
        }
    }

    return false;
}

void GameManager::SetPlayerState(const int playerId, const EPlayerState state)
{
    mPlayers[playerId]->SetCurrentState(state);
}

void GameManager::HideShips() const
{
    for (PlayerPtr player : mPlayers)
    {
        if (player->GetCurrentState() == EPlayerState::Ready)
        {
            player->HideShips();
        }
    }
}