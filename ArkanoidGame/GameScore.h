#pragma once
#include "IObserver.h"

namespace ArkanoidGame {
    class GameScore : public IObserver
    {
    public:
        void Notify(std::shared_ptr<IObservable> observable) override;
        int GetScore() const { return score; }
        void SetScore(int s) { score = s; }
    private:
        int score = 0;
    };
}