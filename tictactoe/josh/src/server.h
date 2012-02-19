#include "ai.h"

class InMemoryGameService : public IGameService
{
    shared_ptr<IArtificialIntelligence> m_intelligence;

    // TODO this would be a cache of running games
    shared_ptr<Game> m_game;

public:
    InMemoryGameService(shared_ptr<IArtificialIntelligence>& intelligence)
    {
        m_intelligence = intelligence;
    }

    static shared_ptr<IGameService> Create()
    {
        auto intelligence = shared_ptr<IArtificialIntelligence>(new BogoArtificialIntelligence(3));
        return shared_ptr<IGameService>(new InMemoryGameService(intelligence));
    }

    int GetWinnerForGameId(int gameId)
    {
        auto game = GetGameById(gameId);
        game->WinningPlayerId = CalculateWinningPlayerId(game->GetTiles());
        return game->WinningPlayerId;
    }

    int Host()
    {
        return 0;
    }

    shared_ptr<Game> Join(int gameId)
    {
        // TODO allocate game instead of overwrite
        m_game = shared_ptr<Game>(new Game());
        return m_game;
    }

    bool Move(int gameId, int playerId, int posX, int posY)
    {
        return m_game->Move(playerId, posX, posY);
    }

    shared_ptr<PlayerAction> WaitForMove(int gameId, int playerId)
    {
        auto game = GetGameById(gameId);
        auto opponentMove = m_intelligence->ComputeSuggestedAction(game, 2);
        m_game->Move(opponentMove->PlayerId, opponentMove->PosX, opponentMove->PosY);
        return opponentMove;
    }

private:
    int CalculateWinningPlayerId(vector<vector<int>> tiles)
    {
        // uh brute force this for now since i'm too lazy to come up with something elegant
        // check possible combinations from the center
        int possibleWinner = tiles[1][1];
        if (possibleWinner != 0)
        {
            if ((tiles[1][0] == possibleWinner && tiles[1][2] == possibleWinner) 
                || (tiles[0][1] == possibleWinner && tiles[2][1] == possibleWinner)
                || (tiles[0][0] == possibleWinner && tiles[2][2] == possibleWinner)
                || (tiles[0][2] == possibleWinner && tiles[2][0] == possibleWinner))
            {
                return possibleWinner;
            }
        }
        
        // check possible combinations from top left corner
        possibleWinner = tiles[0][0];
        if (possibleWinner != 0)
        {
            if ((tiles[0][1] == possibleWinner && tiles[0][2] == possibleWinner) 
                || (tiles[1][0] == possibleWinner && tiles[2][0] == possibleWinner))
            {
                return possibleWinner;
            }
        }

        // check possible combinations from bottom right corner
        possibleWinner = tiles[2][2];
        if (possibleWinner != 0)
        {
            if ((tiles[0][2] == possibleWinner && tiles[1][2] == possibleWinner) 
                || (tiles[2][0] == possibleWinner && tiles[2][1] == possibleWinner))
            {
                return possibleWinner;
            }
        }

        return 0;
    }

    shared_ptr<Game> GetGameById(int gameId)
    {
        // TODO this would be a lookup of the running game
        return m_game;
    }

};
