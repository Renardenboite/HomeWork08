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
		std::map<int, std::string> sortedRecordsTable;
		for (const auto& item : game.GetRecordsTable())
		{
			sortedRecordsTable[item.second] = item.first;
		}

		auto it = sortedRecordsTable.rbegin();
		for (int i = 0; i < SETTINGS.MAX_RECORDS_TABLE_SIZE && it != sortedRecordsTable.rend(); ++i, ++it) // Note, we can do several actions in for action block
		{			
			auto text = std::make_unique<sf::Text>();

			std::stringstream sstream;
			sstream << i + 1 << ". " << it->second << ": " << it->first;
			text->setString(sstream.str());
			text->setFont(font);
			text->setFillColor(sf::Color::White);
			text->setCharacterSize(24);
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

	void GameStateRecordsData::Update(float timeDelta)
	{
	}

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

		sf::Vector2f tablePosition = { titleText.getGlobalBounds().left, viewSize.y / 2.f };
		DrawTextList(window, textsList, 10.f, Orientation::Vertical, Alignment::Min, tablePosition, { 0.f, 0.f });

		hintText.setOrigin(GetTextOrigin(hintText, { 0.5f, 1.f }));
		hintText.setPosition(viewSize.x / 2.f, viewSize.y - 50.f);
		window.draw(hintText);
	}

}
