#include "GameScore.h"
#include "Block.h"
#include "ThreeHitBlock.h"

namespace ArkanoidGame {
    void GameScore::Notify(std::shared_ptr<IObservable> observable) {
        if (auto block = std::dynamic_pointer_cast<Block>(observable)) {
            if (dynamic_cast<ThreeHitBlock*>(block.get()))
                score += 30;
            else
                score += 10;
        }
    }
}