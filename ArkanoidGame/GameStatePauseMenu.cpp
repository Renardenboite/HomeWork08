#include "GameStatePauseMenu.h"
#include "Application.h"
#include "Game.h"
#include <assert.h>

namespace ArkanoidGame
{
	void GameStatePauseMenuData::Init()
	{
		const sf::Font& font = Application::Instance().GetGame().GetDefaultFont();

		background.setFillColor(sf::Color(0, 0, 0, 128)); // Semi-transparent black

		titleText.setString("Pause");
		titleText.setFont(font);
		titleText.setCharacterSize(48);
		titleText.setFillColor(sf::Color::Red);

		auto resumeItem = std::make_unique<MenuItem>();
		resumeItem->text->setString("Return to game");
		resumeItem->text->setFont(font);
		resumeItem->text->setCharacterSize(24);
		resumeItem->onPressCallback = [](MenuItem*, Menu*) {
			Application::Instance().GetGame().PopState();
			};

		auto saveItem = std::make_unique<MenuItem>();
		saveItem->text->setString("Save Game");
		saveItem->text->setFont(font);
		saveItem->text->setCharacterSize(24);
		saveItem->onPressCallback = [](MenuItem*, Menu*) {
			Application::Instance().GetGame().SaveCurrentGame("save.dat");
			};

		auto exitItem = std::make_unique<MenuItem>();
		exitItem->text->setString("Exit to main menu");
		exitItem->text->setFont(font);
		exitItem->text->setCharacterSize(24);
		exitItem->onPressCallback = [](MenuItem*, Menu*) { Application::Instance().GetGame().ExitGame(); };

		MenuItem pauseMenu;
		pauseMenu.childrenOrientation = Orientation::Vertical;
		pauseMenu.childrenAlignment = Alignment::Middle;
		pauseMenu.childrens.push_back(std::move(resumeItem));
		pauseMenu.childrens.push_back(std::move(saveItem));
		pauseMenu.childrens.push_back(std::move(exitItem));

		menu.Init(pauseMenu, font);
	}

	void GameStatePauseMenuData::HandleWindowEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				Application::Instance().GetGame().PopState();
			}

			if (event.key.code == sf::Keyboard::Enter)
			{
				menu.PressOnSelectedItem();
			}

			Orientation orientation = menu.GetCurrentContext().childrenOrientation;
			if (event.key.code == sf::Keyboard::Up)
			{
				menu.SwitchToPreviousMenuItem();
			}
			else if (event.key.code == sf::Keyboard::Down)
			{
				menu.SwitchToNextMenuItem();
			}
		}
	}

	void GameStatePauseMenuData::Update(float timeDelta)
	{

	}

	void GameStatePauseMenuData::Draw(sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = (sf::Vector2f)window.getView().getSize();
		
		background.setSize(viewSize);
		window.draw(background);

		titleText.setOrigin(GetTextOrigin(titleText, { 0.5f, 0.f }));
		titleText.setPosition(viewSize.x / 2.f, 100);
		window.draw(titleText);

		menu.Draw(window, window.getView().getCenter(), { 0.5f, 0.f });
	}

}
