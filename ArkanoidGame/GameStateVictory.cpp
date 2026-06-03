#include "GameStateVictory.h"
#include "Application.h"
#include "Game.h"
#include "Text.h"
#include <assert.h>
#include <sstream>

namespace ArkanoidGame
{
    void GameStateVictoryData::Init()
    {
        const sf::Font& font = Application::Instance().GetGame().GetDefaultFont();

        victoryText.setFont(font);
        victoryText.setString("YOU WIN! CONGRATULATIONS!");
        victoryText.setCharacterSize(48);
        victoryText.setFillColor(sf::Color::Green);

        const Game& game = Application::Instance().GetGame();
        const auto& records = game.GetRecordsTable();
        std::vector<std::pair<std::string, int>> sorted(records.begin(), records.end());
        std::sort(sorted.begin(), sorted.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });

        recordsTableTexts.clear();
        size_t count = std::min(sorted.size(), static_cast<size_t>(SETTINGS.MAX_RECORDS_TABLE_SIZE));
        for (size_t i = 0; i < count; ++i)
        {
            auto text = std::make_unique<sf::Text>();
            std::stringstream ss;
            ss << (i + 1) << ". " << sorted[i].first << ": " << sorted[i].second;
            text->setString(ss.str());
            text->setFont(font);
            text->setCharacterSize(24);
            text->setFillColor(sorted[i].first == SETTINGS.PLAYER_NAME ? sf::Color::Green : sf::Color::White);
            recordsTableTexts.push_back(std::move(text));
        }

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

        std::vector<sf::Text*> textsList;
        for (auto& t : recordsTableTexts) textsList.push_back(t.get());
        sf::Vector2f tablePos = { viewSize.x / 2.f, viewSize.y / 2.f - 50.f };
        DrawTextList(window, textsList, 10.f, Orientation::Vertical, Alignment::Middle, tablePos, { 0.5f, 0.f });

        hintText.setOrigin(GetTextOrigin(hintText, { 0.5f, 1.f }));
        hintText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f - 200.f);
        window.draw(hintText);
    }
}