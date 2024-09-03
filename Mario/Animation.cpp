#include "Animation.h"

void Animation::update(float deltaSeconds)
{
	currentFrameIndex += speed * deltaSeconds;

	if (static_cast<int>(currentFrameIndex) >= frames.size())
	{
		currentFrameIndex = 0;
	}
}

int Animation::getCurrentFrameIndex()
{
	return static_cast<int>(currentFrameIndex);
}

const sf::IntRect& Animation::getCurrentFrame()
{
	return frames[getCurrentFrameIndex()];
}