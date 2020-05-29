#include "GameManager.h"

int main()
{
    GameManager::Create();

    while (!GameManager::GetQuitStatus())
    {
        GameManager::Update();
    }

    GameManager::Shutdown();
}


