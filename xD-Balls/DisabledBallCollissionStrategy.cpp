#include "DisabledBallCollissionStrategy.hpp"



DisabledBallCollissionStrategy::DisabledBallCollissionStrategy() : BallCollissionStrategy("disabled")
{
}


DisabledBallCollissionStrategy::~DisabledBallCollissionStrategy() = default;

void DisabledBallCollissionStrategy::handle(std::vector<Ball>& balls) {}
