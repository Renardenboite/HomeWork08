#include "GameStateRecords.h"
#include "Application.h"
#include "Text.h"
#include "Game.h"
#include "GameSettings.h"
#include <assert.h>
#include <sstream>

namespace ArkanoidGame
{
	void GameStateRecordsData::Init()
	{
		const sf::Font& font = Application::Instance().GetGame().GetDefaultFont();

		titleText.setString("RECORDS");
		titleText.setFont(font);
		titleText.setFillColor(sf::Color::Red);
		titleText.setCharacterSize(48);

		tableTexts.reserve(SETTINGS.MAX_RECORDS_TABLE_SIZE);

		const Game& game = Application::Instance().GetGame();
		const auto& records = game.GetRecordsTable();

		std::vector<std::pair<std::string, int>> sorted;
		for (const auto& [name, score] : records) {
			sorted.emplace_back(name, score);
		}
		
		
		std::sort(sorted.begin(), sorted.end(),
			[](const auto& a, const auto& b) {
				return a.second > b.second;
			});

		tableTexts.clear();
		size_t count = std::min(sorted.size(), static_cast<size_t>(SETTINGS.MAX_RECORDS_TABLE_SIZE));

		for (size_t i = 0; i < count; ++i)
		{
			auto text = std::make_unique<sf::Text>();
			std::stringstream ss;
			ss << (i + 1) << ". " << sorted[i].first << ": " << sorted[i].second;
			text->setString(ss.str());
			text->setFont(font);
			text->setCharacterSize(24);
			text->setFillColor(sf::Color::White);

			tableTexts.push_back(std::move(text));
		}

		hintText.setString("Press ESC to return back to main menu");
		hintText.setFont(font);
		hintText.setFillColor(sf::Color::White);
		hintText.setCharacterSize(24);
	}

	void GameStateRecordsData::HandleWindowEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				Application::Instance().GetGame().PopState();
			}
		}
	}

	void GameStateRecordsData::Update(float timeDelta) {}

	void GameStateRecordsData::Draw(sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = window.getView().getSize();

		titleText.setOrigin(GetTextOrigin(titleText, { 0.5f, 0.f }));
		titleText.setPosition(viewSize.x / 2.f, 50.f);
		window.draw(titleText);

		std::vector<sf::Text*> textsList;
		textsList.reserve(tableTexts.size());
		for (auto& text : tableTexts)
		{
			textsList.push_back(text.get());
		}

		sf::Vector2f tablePosition = { viewSize.x / 2.f, 150.f };
		DrawTextList(window, textsList, 10.f, Orientation::Vertical, Alignment::Middle, tablePosition, { 0.5f, 0.f });

		hintText.setOrigin(GetTextOrigin(hintText, { 0.5f, 1.f }));
		hintText.setPosition(viewSize.x / 2.f, viewSize.y - 50.f);
		window.draw(hintText);
	}

}
