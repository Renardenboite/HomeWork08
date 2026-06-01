#include "GameStatePlaying.h"
#include "Application.h"
#include "Block.h"
#include "Game.h"
#include "Text.h"
#include "ThreeHitBlock.h"
#include "randomizer.h"
#include "BonusFactory.h"
#include <assert.h>
#include <sstream>

namespace ArkanoidGame
{
    void GameStatePlayingData::Init()
    {
        font = &Application::Instance().GetGame().GetDefaultFont();
        assert(gameOverSoundBuffer.loadFromFile(SETTINGS.SOUNDS_PATH + "Death.wav"));
        gameOverSound.setBuffer(gameOverSoundBuffer);

        factories.emplace(BlockType::Simple, std::make_unique<SimpleBlockFactory>());
        factories.emplace(BlockType::ThreeHit, std::make_unique<ThreeHitBlockFactory>());
        factories.emplace(BlockType::Unbreackable, std::make_unique<UnbreackableBlockFactory>());

        background.setSize(sf::Vector2f(static_cast<float>(SETTINGS.SCREEN_WIDTH),
                                        static_cast<float>(SETTINGS.SCREEN_HEIGHT)));
        background.setPosition(0.f, 0.f);
        background.setFillColor(sf::Color(30, 30, 30));

        gameScore = std::make_shared<GameScore>();

        scoreText.setFont(*font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::Yellow);

        livesText.setFont(*font);
        livesText.setCharacterSize(24);
        livesText.setFillColor(sf::Color::White);

        inputHintText.setFont(*font);
        inputHintText.setCharacterSize(24);
        inputHintText.setFillColor(sf::Color::White);
        inputHintText.setString("Use arrow keys to move, ESC to pause");
        inputHintText.setOrigin(GetTextOrigin(inputHintText, { 1.f, 0.f }));

        auto platformObj = std::make_shared<Platform>(
            sf::Vector2f{ SETTINGS.SCREEN_WIDTH / 2.f, SETTINGS.SCREEN_HEIGHT - SETTINGS.PLATFORM_HEIGHT / 2.f });
        gameObjects.push_back(platformObj);
        platform = platformObj.get();

        auto ballObj = std::make_shared<Ball>(
            sf::Vector2f{ SETTINGS.SCREEN_WIDTH / 2.f, SETTINGS.SCREEN_HEIGHT - SETTINGS.PLATFORM_HEIGHT - SETTINGS.BALL_SIZE / 2.f });
        ballObj->AddObserver(weak_from_this());
        balls.push_back(ballObj);
        gameObjects.push_back(ballObj);

        createBlocks();
        lives = 3;
    }

    bool GameStatePlayingData::onBallLost(std::shared_ptr<Ball> lostBall)
    {
        if (balls.size() > 1) return true;
        lives--;

        if (lives > 0 && font)
        {
            sf::Text lifeText;
            lifeText.setFont(*font);
            lifeText.setString("Life lost! Lives remaining: " + std::to_string(lives));
            lifeText.setCharacterSize(28);
            lifeText.setFillColor(sf::Color::Red);
            sf::FloatRect bounds = lifeText.getLocalBounds();
            lifeText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
            lifeText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f, SETTINGS.SCREEN_HEIGHT / 2.f + 50.f);
            activeLifeMessages.push_back({ lifeText, 2.0f });
        }

        if (lives <= 0)
        {
            gameOverSound.play();
            Application::Instance().GetGame().LooseGame();
            return true;
        }
        else
        {
            lostBall->restart();
            lostBall->SetPosition({
                SETTINGS.SCREEN_WIDTH / 2.f,
                SETTINGS.SCREEN_HEIGHT - SETTINGS.PLATFORM_HEIGHT - SETTINGS.BALL_SIZE / 2.f
                });
            return false;
        }
    }

    void GameStatePlayingData::HandleWindowEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
            if (event.key.code == sf::Keyboard::Escape)
                Application::Instance().GetGame().PauseGame();
    }

    void GameStatePlayingData::Update(float timeDelta)
    {
        for (auto& obj : gameObjects) obj->Update(timeDelta);
        for (auto& block : blocks) block->Update(timeDelta);
        updateBonuses(timeDelta);

        auto platform = std::dynamic_pointer_cast<Platform>(gameObjects[0]);
        if (!platform) return;

        const int microSteps = 100;
        float stepDelta = timeDelta / microSteps;

        for (auto ballIt = balls.begin(); ballIt != balls.end(); )
        {
            auto ball = *ballIt;
            if (!ball) { ++ballIt; continue; }

            bool bounced = false;

            if (platform->CheckCollision(ball))
            {
                ball->InvertDirectionY();
                sf::Vector2f pos = ball->GetPosition();
                pos.y = platform->GetRect().top - SETTINGS.BALL_SIZE / 2.f - 2.f;
                ball->SetPosition(pos);
            }

            bool ballLost = false;
            for (int step = 0; step < microSteps; ++step)
            {
                sf::Vector2f oldPos = ball->GetPosition();
                ball->Update(stepDelta);
                sf::Vector2f newPos = ball->GetPosition();

                if (platform->CheckCollision(ball))
                {
                    ball->InvertDirectionY();
                    sf::Vector2f pos = ball->GetPosition();
                    pos.y = platform->GetRect().top - SETTINGS.BALL_SIZE / 2.f - 2.f;
                    ball->SetPosition(pos);
                    continue;
                }

                bool collided = false;
                for (auto& block : blocks)
                {
                    if (block->IsDestroyed() || block->IsTimerStarted()) continue;

                    if (block->CheckCollision(ball))
                    {
                        collided = true;
                        sf::FloatRect blockRect = block->GetRect();
                        const float BALL_RADIUS = SETTINGS.BALL_SIZE / 2.0f;
                        sf::Vector2f ballCenter = newPos;

                        float overlapLeft = ballCenter.x + BALL_RADIUS - blockRect.left;
                        float overlapRight = blockRect.left + blockRect.width - (ballCenter.x - BALL_RADIUS);
                        float overlapTop = ballCenter.y + BALL_RADIUS - blockRect.top;
                        float overlapBottom = blockRect.top + blockRect.height - (ballCenter.y - BALL_RADIUS);

                        float minX = std::min(overlapLeft, overlapRight);
                        float minY = std::min(overlapTop, overlapBottom);

                        sf::Vector2f push;
                        bool invX = false, invY = false;

                        if (minX < minY)
                        {
                            push.x = (overlapLeft < overlapRight) ? -overlapLeft : overlapRight;
                            push.y = 0.f;
                            invX = true;
                        }
                        else
                        {
                            push.x = 0.f;
                            push.y = (overlapTop < overlapBottom) ? -overlapTop : overlapBottom;
                            invY = true;
                        }

                        float extra = dynamic_cast<UnbreackableBlock*>(block.get()) ? 12.0f : 4.5f;
                        push.x += (push.x > 0 ? extra : (push.x < 0 ? -extra : 0.f));
                        push.y += (push.y > 0 ? extra : (push.y < 0 ? -extra : 0.f));

                        ball->SetPosition(ball->GetPosition() + push);

                        if (dynamic_cast<UnbreackableBlock*>(block.get()))
                        {
                            sf::Vector2f blockCenter(blockRect.left + blockRect.width / 2.f,
                                blockRect.top + blockRect.height / 2.f);
                            sf::Vector2f newDir = ball->GetPosition() - blockCenter;
                            float len = std::sqrt(newDir.x * newDir.x + newDir.y * newDir.y);
                            if (len > 0.01f)
                            {
                                newDir /= len;
                                ball->SetDirection(newDir);
                            }
                            else
                            {
                                ball->SetDirection(sf::Vector2f(0.f, -1.f));
                            }
                            sf::FloatRect r = blockRect;
                            r.left -= 4.f; r.top -= 4.f; r.width += 8.f; r.height += 8.f;
                            if (r.contains(ball->GetPosition()))
                            {
                                sf::Vector2f dir = ball->GetPosition() - blockCenter;
                                float len2 = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                                if (len2 > 0.01f) dir /= len2;
                                else dir = sf::Vector2f(0.f, -1.f);
                                ball->SetPosition(blockCenter + dir * (std::max(r.width, r.height) / 2.f + BALL_RADIUS + 10.f));
                            }
                        }
                        else
                        {
                            if (invX && !bounced)
                            {
                                ball->InvertDirectionX();
                                ball->UpdateAngleFromDirection();
                                ball->AddAngleDeviation(random<float>(-15.f, 15.f));
                                bounced = true;
                            }
                            if (invY && !bounced)
                            {
                                ball->InvertDirectionY();
                                ball->UpdateAngleFromDirection();
                                ball->AddAngleDeviation(random<float>(-15.f, 15.f));
                                bounced = true;
                            }
                        }

                        if (!dynamic_cast<UnbreackableBlock*>(block.get()))
                        {
                            sf::FloatRect expandedRect = blockRect;
                            expandedRect.left -= 2.f; expandedRect.top -= 2.f;
                            expandedRect.width += 4.f; expandedRect.height += 4.f;
                            if (expandedRect.contains(ball->GetPosition()))
                            {
                                sf::Vector2f center(blockRect.left + blockRect.width / 2.f,
                                    blockRect.top + blockRect.height / 2.f);
                                sf::Vector2f dir = ball->GetPosition() - center;
                                float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                                if (len < 0.01f) { dir = sf::Vector2f(0.f, -1.f); len = 1.0f; }
                                dir /= len;
                                float pushOutDist = BALL_RADIUS + std::sqrt(blockRect.width * blockRect.width +
                                    blockRect.height * blockRect.height) / 2.f + 6.f;
                                ball->SetPosition(center + dir * pushOutDist);
                            }
                        }

                        break;
                    }
                }

                if (ball->GetPosition().y > SETTINGS.SCREEN_HEIGHT + SETTINGS.BALL_SIZE)
                {
                    if (onBallLost(ball))
                        ballIt = balls.erase(ballIt);
                    else
                        ++ballIt;
                    ballLost = true;
                    break;
                }
            }

            if (!ballLost) ++ballIt;
        }

        int removedBreakable = 0;
        blocks.erase(
            std::remove_if(blocks.begin(), blocks.end(),
                [&](const std::shared_ptr<Block>& b) {
                    if (b->IsDestroyed() && !b->IsTimerStarted()) {
                        if (!dynamic_cast<UnbreackableBlock*>(b.get()))
                            ++removedBreakable;
                        return true;
                    }
                    return false;
                }),
            blocks.end()
        );
        breakableBlocksCount -= removedBreakable;
        if (breakableBlocksCount <= 0)
        {
            pendingLevelLoad = true;
            breakableBlocksCount = 0;
        }

        updateActiveEffects(timeDelta);

        for (auto it = activeBonusTexts.begin(); it != activeBonusTexts.end(); ) 
        {
            it->remainingTime -= timeDelta;
            if (it->remainingTime <= 0.f)
                it = activeBonusTexts.erase(it);
            else
                ++it;
        }

        for (auto it = activeLifeMessages.begin(); it != activeLifeMessages.end(); ) 
        {
            it->remainingTime -= timeDelta;
            if (it->remainingTime <= 0.f)
                it = activeLifeMessages.erase(it);
            else
                ++it;
        }

        scoreText.setString("Score: " + std::to_string(gameScore->GetScore()));
        livesText.setString("Lives: " + std::to_string(lives));

        if (pendingLevelLoad)
        {
            pendingLevelLoad = false;
            LoadNextLevel();
        }
    }

    void GameStatePlayingData::Draw(sf::RenderWindow& window)
    {
        window.draw(background);
        for (auto& obj : gameObjects) obj->Draw(window);
        for (auto& block : blocks) block->Draw(window);
        for (auto& bonus : bonuses) bonus->Draw(window);
        drawUI(window);
    }

    void GameStatePlayingData::LoadNextLevel()
    {
        if (currentLevel >= levelLoader.GetLevelCount() - 1)
        {
            Application::Instance().GetGame().WinGame();
            return;
        }

        auto platform = std::dynamic_pointer_cast<Platform>(gameObjects[0]);
        auto ball = std::dynamic_pointer_cast<Ball>(gameObjects[1]);
        platform->restart();
        ball->restart();

        blocks.clear();
        ++currentLevel;
        createBlocks();
    }

    void GameStatePlayingData::Notify(std::shared_ptr<IObservable> observable)
    {
        if (auto block = std::dynamic_pointer_cast<Block>(observable))
        {
            if (!dynamic_cast<UnbreackableBlock*>(block.get()))
            {
                auto percent = random<int>(0, 100);
                if (SETTINGS.BONUS_PROPABILITY_PERCENT >= percent)
                {
                    auto bonus = BonusFactory::CreateRandom(block->GetPosition());
                    if (bonus) bonuses.push_back(std::move(bonus));
                }
            }
        }
    }

    void GameStatePlayingData::createBlocks()
    {
        for (auto& pair : factories) pair.second->ClearCounter();
        auto self = weak_from_this();
        auto level = levelLoader.GetLevel(currentLevel);

        const int cols = level.width;
        const int rows = level.height;
        if (cols <= 0 || rows <= 0) return;

        const float gapX = static_cast<float>(SETTINGS.BLOCK_SHIFT);
        const float gapY = static_cast<float>(SETTINGS.BLOCK_SHIFT);
        const float blockHeight = SETTINGS.BLOCK_HEIGHT;
        const float blockWidth = (SETTINGS.SCREEN_WIDTH - (cols + 1) * gapX) / cols;

        const float startX = gapX + blockWidth / 2.f;
        const float startY = 80.f;

        for (auto& [pos, blockType] : level.m_blocks)
        {
            float x = startX + pos.x * (blockWidth + gapX);
            float y = startY + pos.y * (blockHeight + gapY);

            auto block = factories.at(blockType)->CreateBlock(sf::Vector2f{ x, y }, blockWidth, blockHeight);
            block->AddObserver(self);
            block->AddObserver(gameScore);
            blocks.push_back(block);
        }

        breakableBlocksCount = 0;
        for (auto& pair : factories)
            breakableBlocksCount += pair.second->GetcreatedBreackableBlocksCount();
    }

    void GameStatePlayingData::GetBallInverse(const sf::Vector2f& ballPos, const sf::FloatRect& blockRect,
        bool& needInverseDirX, bool& needInverseDirY)
    {
        if (ballPos.y > blockRect.top + blockRect.height) needInverseDirY = true;
        if (ballPos.x < blockRect.left) needInverseDirX = true;
        if (ballPos.x > blockRect.left + blockRect.width) needInverseDirX = true;
    }

    void GameStatePlayingData::updateBonuses(float timeDelta)
    {
        for (auto& bonus : bonuses)
        {
            bonus->Update(timeDelta);
            if (bonus->IsCaught(*platform))
            {
                auto effect = bonus->TakeEffect();
                if (effect)
                {
                    effect->Apply(*this);
                    std::string description = effect->GetDescription();
                    sf::Text bonusText;
                    bonusText.setFont(*font);
                    bonusText.setString(description);
                    bonusText.setCharacterSize(30);
                    bonusText.setFillColor(sf::Color::White);
                    sf::FloatRect bounds = bonusText.getLocalBounds();
                    bonusText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
                    bonusText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f, SETTINGS.SCREEN_HEIGHT / 2.f);
                    activeBonusTexts.push_back({ bonusText, 2.0f });

                    activeEffects.emplace_back(std::move(effect), 7.0f);
                }
                bonus->Destroy();
            }
        }
        bonuses.erase(std::remove_if(bonuses.begin(), bonuses.end(),
            [](const auto& b) { return b->IsDestroyed(); }), bonuses.end());
    }

    void GameStatePlayingData::updateActiveEffects(float timeDelta)
    {
        for (auto it = activeEffects.begin(); it != activeEffects.end(); )
        {
            it->remainingTime -= timeDelta;
            if (it->remainingTime <= 0.f)
            {
                it->effect->Revert(*this);
                it = activeEffects.erase(it);
            }
            else ++it;
        }
    }

    void GameStatePlayingData::drawUI(sf::RenderWindow& window)
    {
        scoreText.setPosition(10.f, 10.f);
        window.draw(scoreText);
        livesText.setPosition(10.f, 40.f);
        window.draw(livesText);
        sf::Vector2f viewSize = window.getView().getSize();
        inputHintText.setPosition(viewSize.x - 10.f, 10.f);
        for (auto& t : activeBonusTexts)
            window.draw(t.text);
        for (auto& msg : activeLifeMessages)
            window.draw(msg.text);
        window.draw(inputHintText);
    }

    void GameStatePlayingData::AddBall(std::shared_ptr<Ball> ball)
    {
        balls.push_back(ball);
        gameObjects.push_back(ball);
    }

    void GameStatePlayingData::RemoveBall(std::shared_ptr<Ball> ball)
    {
        balls.erase(std::remove(balls.begin(), balls.end(), ball), balls.end());
        gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), ball), gameObjects.end());
    }

    void GameStatePlayingData::AddBonus(std::unique_ptr<Bonus> bonus)
    {
        bonuses.push_back(std::move(bonus));
    }

    void GameStatePlayingData::AddActiveEffect(std::unique_ptr<BonusEffect> effect, float duration)
    {
        activeEffects.emplace_back(std::move(effect), duration);
    }
}