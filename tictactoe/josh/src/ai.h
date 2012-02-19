#include <cstdlib>
#include <ctime>

class IArtificialIntelligence
{
public:
    virtual PlayerAction* ComputeSuggestedAction(Game* game, int playerId) = 0;
};

class BogoArtificialIntelligence : public IArtificialIntelligence
{
    int m_boardSize;

public:
    BogoArtificialIntelligence(int boardSize)
    {
        srand(time(NULL));
        m_boardSize = boardSize;
    }

    PlayerAction* ComputeSuggestedAction(Game* game, int playerId)
    {
        PlayerAction* opponentAction = new PlayerAction;
        opponentAction->PlayerId = playerId;

        bool isValidAction = false;

        while (!isValidAction)
        {
            int randX = rand() % m_boardSize;
            opponentAction->PosX = randX;

            int randY = rand() % m_boardSize;
            opponentAction->PosY = randY;

            isValidAction = *game->GetTiles()[randX][randY] == 0;
        }

        return opponentAction; 
    }
};
