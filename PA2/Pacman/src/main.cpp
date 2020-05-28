#include <string>
#include "Gameplay/Game.h"

int main(void)
{
    Game * g = new Game("examples/Game");
    g->Init();
    g->Play();
    delete g;
    return 0;
}
