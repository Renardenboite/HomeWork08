#pragma once
#include "IObserver.h"

namespace ArkanoidGame {
    class GameScore : public IObserver
    {
    public:
        void Notify(std::shared_ptr<IObservable> observable) override;
        int GetScore() const { return score; }
    private:
        int score = 0;
    };
}