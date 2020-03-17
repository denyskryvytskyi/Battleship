#include "GameManager.h"
#include "PlayerFactory.h"
#include "Config.h"
#include "CommonTypes.h"

GameManager::GameManager(const int mode) :
    mMainWindow(sf::VideoMode(cfg::mode_width, cfg::mode_height), "Battleship"),
    mSecondPlayerPosOffset(cfg::mode_width / 2),
    mGameMode((EGameMode)mode),
    mGameState(EGameState::PlacingShips)
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
    //else if (mGameMode == EGameMode::OnePlayer)
    //{
    //    PlayerPtr playerAi = PlayerFactory::Create(sf::FloatRect(0.5f, 0, 0.5f, 1), playerField, "AI", false, true); // create AiPlayer
    //    playerAi->InitShips(); // автоматически расставить для ИИ
    //    //playerAi->setMapPlayerAiColor();
    //    mPlayers.push_back(playerAi);
    //}

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
        Render();
    }
}

void GameManager::Update()
{
    for (PlayerPtr player : mPlayers)
    {
        player->Update();
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
                    if (!TryPlacing(pos))
                    {
                        mGameState = EGameState::Battle;
                        mPlayers[0]->SetCurrentState(EPlayerState::Fire);
                        mPlayers[1]->SetCurrentState(EPlayerState::Wait);
                    }
                }
                if (mGameState == EGameState::Battle)
                {
                    // Battle
                    TryFire(pos);
                }
            }
        }

        if (event.mouseButton.button == KeyCode_H)
        {
            HideShips();
        }
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
            mMainWindow.draw(player->GetCell(i, j)); // draw one cell
        }

    player->mName.setPosition(player->mCamera.getCenter().x - 25, 0);
    mMainWindow.draw(player->mName);
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
            return true;
        }
    }

    return false;
}

void GameManager::HideShips() const
{
    for (PlayerPtr player : mPlayers)
    {
        if (player->GetCurrentState() == EPlayerState::PlacingFinished)
        {
            player->HideShips();
        }
    }
}

//void GameManager::RenderGameOver(short winner)
//{
//    sf::RenderWindow win(sf::VideoMode(250, 60), "Battleship winner");
//
//    while (win.isOpen())
//    {
//        Event event;
//        while (win.pollEvent(event))
//        {
//            if (event.type == Event::Closed || m_GameMode == e_GameMode::exitGame)
//                win.close();
//        }
//        Font font;
//        font.loadFromFile("arial.ttf");
//        Text t;
//        t.setFont(font);
//        t.setFont(font);
//        t.setFillColor(Color::Red);
//
//        t.setCharacterSize(20);
//        string str;
//
//        switch (winner)
//        {
//        case 1: str = "Player 1";
//            break;
//        case 2:	str = "Player 2";
//            break;
//        case 3: str = "Computer";
//            break;
//        }
//        str += " is winner!";
//        t.setString(str);
//
//        win.clear();
//        win.draw(t);
//        win.display();
//    }
//}