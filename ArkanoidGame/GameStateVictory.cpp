#include "GameStateVictory.h"
#include "Application.h"
#include "Game.h"
#include "Text.h"
#include <assert.h>

namespace ArkanoidGame
{
    void GameStateVictoryData::Init()
    {
        const sf::Font& font = Application::Instance().GetGame().GetDefaultFont();

        victoryText.setFont(font);
        victoryText.setString("YOU WIN! CONGRATULATIONS!");
        victoryText.setCharacterSize(48);
        victoryText.setFillColor(sf::Color::Green);

        hintText.setFont(font);
        hintText.setString("Press Enter to play again\nEsc to return to main menu");
        hintText.setCharacterSize(24);
        hintText.setFillColor(sf::Color::White);
    }

    void GameStateVictoryData::HandleWindowEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Enter)
            {
                Application::Instance().GetGame().StartGame();
            }
            else if (event.key.code == sf::Keyboard::Escape)
            {
                Application::Instance().GetGame().ExitGame();
            }
        }
    }

    void GameStateVictoryData::Update(float timeDelta) {}

    void GameStateVictoryData::Draw(sf::RenderWindow& window)
    {
        sf::Vector2f viewSize = window.getView().getSize();

        victoryText.setOrigin(GetTextOrigin(victoryText, { 0.5f, 0.5f }));
        victoryText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f - 50.f);
        window.draw(victoryText);

        hintText.setOrigin(GetTextOrigin(hintText, { 0.5f, 0.5f }));
        hintText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f + 50.f);
        window.draw(hintText);
    }
}