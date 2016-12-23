#pragma once

namespace kk
{
	enum gameState { STATE_MENU, STATE_PLAYING, STATE_PAUSED, STATE_LOADING, STATE_QUIT };
	void setState(gameState state);
	gameState getState();
}

// State change events

struct evPause {};
struct evResume {};
struct evQuit {};