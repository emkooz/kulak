#include "state.hpp"

namespace kk
{
	static gameState currentState = STATE_PLAYING;

	void setState(gameState state)
	{
		currentState = state;
	}

	gameState getState()
	{
		return currentState;
	}
}