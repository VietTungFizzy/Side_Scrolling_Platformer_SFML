#include"Game.h"

int main()
{
	Game game;
	while (game.GetWindow()->IsWindowClosed() == false)
	{
		game.Update();
		game.Render();
		game.RestartClock();
	}
	return 0;
}