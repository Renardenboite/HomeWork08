#include "Game.h"
#include <assert.h>
#include <algorithm>
#include <fstream>
#include <windows.h>
#include "GameStatePlaying.h"
#include "GameStateGameOver.h"
#include "GameStatePauseMenu.h"
#include "GameStateMainMenu.h"
#include "GameStateRecords.h"

namespace ArkanoidGame
{
	Game::Game()
	{
		LoadResources();
		
		recordsTable =
		{
			{"John", SETTINGS.MAX_APPLES / 2},
			{"Jane", SETTINGS.MAX_APPLES / 3 },
			{"Alice", SETTINGS.MAX_APPLES / 4 },
			{"Bob", SETTINGS.MAX_APPLES / 5 },
			{"Clementine", SETTINGS.MAX_APPLES / 5 },
		};

		stateChangeType = GameStateChangeType::None;
		pendingGameStateType = GameStateType::None;
		pendingGameStateIsExclusivelyVisible = false;
		SwitchStateTo(GameStateType::MainMenu);
	}

	Game::~Game()
	{
		Shutdown();
	}

	void Game::HandleWindowEvents(sf::RenderWindow& window)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (!stateStack.empty())
			{
				stateStack.back()->HandleWindowEvent(event);
			}
		}
	}

	bool Game::Update(float timeDelta)
	{
		if (stateChangeType == GameStateChangeType::Switch)
		{
			while (!stateStack.empty())
			{
				stateStack.pop_back();
			}
		}
		else if (stateChangeType == GameStateChangeType::Pop)
		{
			if (!stateStack.empty())
			{
				stateStack.pop_back();
			}
		}

		if (pendingGameStateType != GameStateType::None)
		{
			stateStack.push_back(std::make_unique<GameState>(pendingGameStateType, pendingGameStateIsExclusivelyVisible));

			if (pendingLoadGame && pendingGameStateType == GameStateType::Playing)
			{
				auto* playingData = dynamic_cast<GameStatePlayingData*>(stateStack.back()->GetDataPtr());
				if (playingData)
				{
					bool loaded = playingData->LoadGame("save.dat");
					OutputDebugStringA(loaded ? "LoadGame: success\n" : "LoadGame: FAILED\n");
				}
				else
				{
					OutputDebugStringA("LoadGame: dynamic_cast returned nullptr\n");
				}
				pendingLoadGame = false;
			}
		}

		stateChangeType = GameStateChangeType::None;
		pendingGameStateType = GameStateType::None;
		pendingGameStateIsExclusivelyVisible = false;

		if (!stateStack.empty())
		{
			stateStack.back()->Update(timeDelta);
			return true;
		}

		return false;
	}

	void Game::Draw(sf::RenderWindow& window)
	{
		if (stateStack.size() > 0)
		{
			std::vector<GameState*> visibleGameStates;
			for (auto it = stateStack.rbegin(); it != stateStack.rend(); ++it)
			{
				visibleGameStates.push_back(it->get());
				if ((*it)->IsExclusivelyVisible()) 
				{
					break;
				}
			}

			for (auto it = visibleGameStates.rbegin(); it != visibleGameStates.rend(); ++it)
			{
				(*it)->Draw(window); 
			}
		}
	}

	void Game::Shutdown()
	{
		while (stateStack.size() > 0)
		{
			stateStack.pop_back();
		}

		stateChangeType = GameStateChangeType::None;
		pendingGameStateType = GameStateType::None;
		pendingGameStateIsExclusivelyVisible = false;
	}

	void Game::PushState(GameStateType stateType, bool isExclusivelyVisible)
	{
		pendingGameStateType = stateType;
		pendingGameStateIsExclusivelyVisible = isExclusivelyVisible;
		stateChangeType = GameStateChangeType::Push;
	}

	void Game::PopState()
	{
		pendingGameStateType = GameStateType::None;
		pendingGameStateIsExclusivelyVisible = false;
		stateChangeType = GameStateChangeType::Pop;
	}

	void Game::ExitGame()
	{
		SwitchStateTo(GameStateType::MainMenu);
	}

	void Game::SwitchStateTo(GameStateType newState)
	{
		pendingGameStateType = newState;
		pendingGameStateIsExclusivelyVisible = false;
		stateChangeType = GameStateChangeType::Switch;
	}

	void Game::ShowRecords()
	{
		PushState(GameStateType::Records, true);
	}

	bool Game::IsEnableOptions(GameOptions option) const
	{
		const bool isEnable = ((std::uint8_t)options & (std::uint8_t)option) != (std::uint8_t)GameOptions::Empty;
		return isEnable;
	}

	void Game::SetOption(GameOptions option, bool value)
	{
		if (value) {
			options = (GameOptions)((std::uint8_t)options | (std::uint8_t)option);
		}
		else {
			options = (GameOptions)((std::uint8_t)options & ~(std::uint8_t)option);
		}
	}

	int Game::GetRecordByPlayerId(const std::string& playerId) const
	{
		auto it = recordsTable.find(playerId);
		return it == recordsTable.end() ? 0 : it->second;
	}

	void Game::QuitGame()
	{
		SwitchStateTo(GameStateType::None);
	}

	void Game::UpdateRecord(const std::string& playerId, int score)
	{
		recordsTable[playerId] = std::max(recordsTable[playerId], score);
	}	

	void Game::StartGame()
	{
		SwitchStateTo(GameStateType::Playing);
	}

	void Game::PauseGame()
	{
		PushState(GameStateType::ExitDialoge, false);
	}

	void Game::WinGame()
	{
		PushState(GameStateType::Victory, false);
	}

	void Game::LooseGame()
	{
		PushState(GameStateType::GameOver, false);
	}

	void Game::UpdateGame(float timeDelta, sf::RenderWindow& window)
	{
		HandleWindowEvents(window);
		if (Update(timeDelta))
		{
			window.clear();
			Draw(window);
			window.display();
		}
		else
		{
			window.close();		
		}
	}

	void Game::LoadNextLevel()
	{
		assert(stateStack.back()->GetType() == GameStateType::Playing);
		auto playingData = (stateStack.back()->GetData<GameStatePlayingData>());
		playingData->LoadNextLevel();
	}

	void Game::SaveCurrentGame(const std::string& filename)
	{
		for (auto it = stateStack.rbegin(); it != stateStack.rend(); ++it)
		{
			auto* playingData = dynamic_cast<GameStatePlayingData*>((*it)->GetDataPtr());
			if (playingData)
			{
				playingData->SaveGame(filename);
				return;
			}
		}
	}

	void Game::LoadGame()
	{
		std::ifstream testFile("save.dat");
		if (!testFile.good())
		{
			OutputDebugStringA("ContinueGame: save.dat not found\n");
			return;
		}
		testFile.close();

		pendingLoadGame = true;
		StartGame();
	}
}
