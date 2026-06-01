#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "GameStateData.h"
#include "GameObject.h"
#include "Ball.h"
#include "Platform.h"
#include "Block.h"
#include "Bonus.h"
#include "BonusEffect.h"
#include "GameScore.h"
#include "LevelLoader.h"
#include "BlockFactory.h"
#include "IObserver.h"

#include <vector>
#include <memory>
#include <unordered_map>

namespace ArkanoidGame
{
    struct ActiveEffect
    {
        std::unique_ptr<BonusEffect> effect;
        float remainingTime;
        ActiveEffect(std::unique_ptr<BonusEffect> eff, float duration)
            : effect(std::move(eff)), remainingTime(duration) {
        }
    };

    class GameStatePlayingData : public GameStateData,
        public IObserver,
        public std::enable_shared_from_this<GameStatePlayingData>
    {
    public:
        void Init() override;
        void HandleWindowEvent(const sf::Event& event) override;
        void Update(float timeDelta) override;
        void Draw(sf::RenderWindow& window) override;

        void LoadNextLevel();
        void Notify(std::shared_ptr<IObservable> observable) override;

        Platform* GetPlatform() const { return platform; }
        std::vector<std::shared_ptr<Ball>>& GetBalls() { return balls; }
        void AddBall(std::shared_ptr<Ball> ball);
        void RemoveBall(std::shared_ptr<Ball> ball);
        void AddBonus(std::unique_ptr<Bonus> bonus);
        void AddActiveEffect(std::unique_ptr<BonusEffect> effect, float duration);
    private:
        void createBlocks();
        void updateBonuses(float timeDelta);
        void updateActiveEffects(float timeDelta);
        void drawUI(sf::RenderWindow& window);
        bool onBallLost(std::shared_ptr<Ball> lostBall);

        static void GetBallInverse(const sf::Vector2f& ballPos, const sf::FloatRect& blockRect,
            bool& needInverseDirX, bool& needInverseDirY);

        const sf::Font* font = nullptr;
        sf::SoundBuffer gameOverSoundBuffer;
        sf::Sound gameOverSound;

        std::vector<std::shared_ptr<GameObject>> gameObjects;
        std::vector<std::shared_ptr<Block>> blocks;
        std::vector<std::unique_ptr<Bonus>> bonuses;
        std::vector<ActiveEffect> activeEffects;
        std::vector<std::shared_ptr<Ball>> balls;

        std::unordered_map<BlockType, std::unique_ptr<BlockFactory>> factories;

        LevelLoader levelLoader;
        int currentLevel = 0;

        Platform* platform = nullptr;

        std::shared_ptr<GameScore> gameScore;
        int lives = 3;
        int breakableBlocksCount = 0;

        sf::Text scoreText;
        sf::Text livesText;
        sf::Text inputHintText;
        sf::RectangleShape background;

        bool pendingLevelLoad = false;

        struct ActiveBonusText 
        {
            sf::Text text;
            float remainingTime;
        };
        std::vector<ActiveBonusText> activeBonusTexts;
        std::vector<ActiveBonusText> activeLifeMessages;
    };
}