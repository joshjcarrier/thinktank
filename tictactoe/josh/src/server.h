#include "ai.h"

class InMemoryGameService : public IGameService
{
    IArtificialIntelligence* m_intelligence;

    // TODO this would be a cache of running games
    Game* m_game;

public:
    InMemoryGameService(IArtificialIntelligence* intelligence)
    {
        m_intelligence = intelligence;
    }

    static IGameService* Create()
    {
        return new InMemoryGameService(new BogoArtificialIntelligence(3));
    }

    int GetWinnerForGameId(int gameId)
    {
        Game* game = GetGameById(gameId);
        game->WinningPlayerId = CalculateWinningPlayerId(game->GetTiles());
        return game->WinningPlayerId;
    }

    int Host()
    {
        return 0;
    }

    Game* Join(int gameId)
    {
        // TODO allocate game instead of overwrite
        m_game = new Game();
        return m_game;
    }

    bool Move(int gameId, int playerId, int posX, int posY)
    {
        return m_game->Move(playerId, posX, posY);
    }

    PlayerAction* WaitForMove(int gameId, int playerId)
    {
        PlayerAction* opponentMove = m_intelligence->ComputeSuggestedAction(GetGameById(gameId), 2);
        m_game->Move(opponentMove->PlayerId, opponentMove->PosX, opponentMove->PosY);
        return opponentMove;
    }

private:
    int CalculateWinningPlayerId(int*** tiles)
    {
        // uh brute force this for now since i'm too lazy to come up with something elegant
        // check possible combinations from the center
        int possibleWinner = *tiles[1][1];
        if (possibleWinner != 0)
        {
            if ((*tiles[1][0] == possibleWinner && *tiles[1][2] == possibleWinner) 
                || (*tiles[0][1] == possibleWinner && *tiles[2][1] == possibleWinner)
                || (*tiles[0][0] == possibleWinner && *tiles[2][2] == possibleWinner)
                || (*tiles[0][2] == possibleWinner && *tiles[2][0] == possibleWinner))
            {
                return possibleWinner;
            }
        }
        
        // check possible combinations from top left corner
        possibleWinner = *tiles[0][0];
        if (possibleWinner != 0)
        {
            if ((*tiles[0][1] == possibleWinner && *tiles[0][2] == possibleWinner) 
                || (*tiles[1][0] == possibleWinner && *tiles[2][0] == possibleWinner))
            {
                return possibleWinner;
            }
        }

        // check possible combinations from bottom right corner
        possibleWinner = *tiles[2][2];
        if (possibleWinner != 0)
        {
            if ((*tiles[0][2] == possibleWinner && *tiles[1][2] == possibleWinner) 
                || (*tiles[2][0] == possibleWinner && *tiles[2][1] == possibleWinner))
            {
                return possibleWinner;
            }
        }

        return 0;
    }

    Game* GetGameById(int gameId)
    {
        // TODO this would be a lookup of the running game
        return m_game;
    }

};
