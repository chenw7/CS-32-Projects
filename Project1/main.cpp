#include "Arena.h"
#include "Player.h"
#include "History.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
using namespace std;

int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(10, 12, 40);

      // Play the game
    g.play();
}
