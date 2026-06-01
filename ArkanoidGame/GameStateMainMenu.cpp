#include "GameStateMainMenu.h"
#include "Application.h"
#include "Game.h"
#include <assert.h>

namespace ArkanoidGame
{
	void GameStateMainMenuData::Init()
	{
		const sf::Font& font = Application::Instance().GetGame().GetDefaultFont();

		auto startGame = std::make_unique<MenuItem>();
		startGame->text->setString("Start Game");
		startGame->text->setFont(font);
		startGame->text->setCharacterSize(24);
        startGame->onPressCallback = [](MenuItem*, Menu*) {
            Application::Instance().GetGame().StartGame(); };
		
        const bool isInfiniteApples = Application::Instance().GetGame().IsEnableOptions(GameOptions::InfiniteApples);
        auto optionsInfiniteApplesItem = std::make_unique<MenuItem>();
        optionsInfiniteApplesItem->text->setString("Infinite Apples: " + std::string(isInfiniteApples ? "On" : "Off"));
        optionsInfiniteApplesItem->text->setFont(font);
        optionsInfiniteApplesItem->text->setCharacterSize(24);
        optionsInfiniteApplesItem->onPressCallback = [](MenuItem* item, Menu*) {
            Game& game = Application::Instance().GetGame();
            bool newOptionValue = !game.IsEnableOptions(GameOptions::InfiniteApples);
            game.SetOption(GameOptions::InfiniteApples, newOptionValue);
            item->text->setString("Infinite Apples: " + std::string(newOptionValue ? "On" : "Off")); };

        const bool isWithAcceleration = Application::Instance().GetGame().IsEnableOptions(GameOptions::WithAcceleration);
        auto optionsWithAccelerationItem = std::make_unique<MenuItem>();
        optionsWithAccelerationItem->text->setString("With Acceleration: " + std::string(isWithAcceleration ? "On" : "Off"));
        optionsWithAccelerationItem->text->setFont(font);
        optionsWithAccelerationItem->text->setCharacterSize(24);
        optionsWithAccelerationItem->onPressCallback = [](MenuItem* item, Menu*) {
            Game& game = Application::Instance().GetGame();
            bool newOptionValue = !game.IsEnableOptions(GameOptions::WithAcceleration);
            game.SetOption(GameOptions::WithAcceleration, newOptionValue);
            item->text->setString("With Acceleration: " + std::string(newOptionValue ? "On" : "Off")); };

        auto options = std::make_unique<MenuItem>();
        options->text->setString("Options");
        options->text->setFont(font);
        options->text->setCharacterSize(24);
        options->hintText->setString("Options");
        options->hintText->setFont(font);
        options->hintText->setCharacterSize(48);
        options->hintText->setFillColor(sf::Color::Red);
        options->childrenOrientation = Orientation::Vertical;
        options->childrenAlignment = Alignment::Middle;
        options->childrenSpacing = 10.f;
        options->childrens.push_back(std::move(optionsInfiniteApplesItem));
        options->childrens.push_back(std::move(optionsWithAccelerationItem));

        auto recordsItem = std::make_unique<MenuItem>();
        recordsItem->text->setString("Records");
        recordsItem->text->setFont(font);
        recordsItem->text->setCharacterSize(24);
        recordsItem->onPressCallback = [](MenuItem*, Menu*) {
            Application::Instance().GetGame().ShowRecords(); };

        auto yesItem = std::make_unique<MenuItem>();
        yesItem->text->setString("Yes");
        yesItem->text->setFont(font);
        yesItem->text->setCharacterSize(24);
        yesItem->onPressCallback = [](MenuItem*, Menu*) {
            Application::Instance().GetGame().QuitGame(); };

        auto noItem = std::make_unique<MenuItem>();
        noItem->text->setString("No");
        noItem->text->setFont(font);
        noItem->text->setCharacterSize(24);
        noItem->onPressCallback = [](MenuItem*, Menu* menu) {
            menu->GoBack(); };   //menu.GoBack()

        auto exitGameItem = std::make_unique<MenuItem>();
        exitGameItem->text->setString("Exit Game");
        exitGameItem->text->setFont(font);
        exitGameItem->text->setCharacterSize(24);
        exitGameItem->hintText->setString("Are you sure?");
        exitGameItem->hintText->setFont(font);
        exitGameItem->hintText->setCharacterSize(48);
        exitGameItem->hintText->setFillColor(sf::Color::Red);
        exitGameItem->childrenOrientation = Orientation::Horizontal;
        exitGameItem->childrenAlignment = Alignment::Middle;
        exitGameItem->childrenSpacing = 10.f;
        exitGameItem->childrens.push_back(std::move(yesItem));
        exitGameItem->childrens.push_back(std::move(noItem));

        auto mainMenu = std::make_unique<MenuItem>();
        mainMenu->hintText->setString("Snake Game");
        mainMenu->hintText->setFont(font);
        mainMenu->hintText->setCharacterSize(48);
        mainMenu->hintText->setFillColor(sf::Color::Red);
        mainMenu->childrenOrientation = Orientation::Vertical;
        mainMenu->childrenAlignment = Alignment::Middle;
        mainMenu->childrenSpacing = 10.f;
        mainMenu->childrens.push_back(std::move(startGame));
        mainMenu->childrens.push_back(std::move(options));
        mainMenu->childrens.push_back(std::move(recordsItem));
        mainMenu->childrens.push_back(std::move(exitGameItem));

        menu.Init(*mainMenu, font);
	}

	void GameStateMainMenuData::HandleWindowEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				menu.GoBack();
			}
			else if (event.key.code == sf::Keyboard::Enter)
			{
				menu.PressOnSelectedItem();
			}
			
			Orientation orientation = menu.GetCurrentContext().childrenOrientation;
			if (orientation == Orientation::Vertical && event.key.code == sf::Keyboard::Up ||
				orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::Left)
			{
				menu.SwitchToPreviousMenuItem();
			}
			else if (orientation == Orientation::Vertical && event.key.code == sf::Keyboard::Down ||
						orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::Right)
			{
				menu.SwitchToNextMenuItem();
			}
		}
	}

	void GameStateMainMenuData::Update(float timeDelta)
	{

	}

	void GameStateMainMenuData::Draw(sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = (sf::Vector2f)window.getView().getSize();

		sf::Text* hintText = menu.GetCurrentContext().hintText.get();
		hintText->setOrigin(GetTextOrigin(*hintText, { 0.5f, 0.f }));
		hintText->setPosition(viewSize.x / 2.f, 150.f);
		window.draw(*hintText);

		menu.Draw(window, viewSize / 2.f, { 0.5f, 0.f });
	}

}
