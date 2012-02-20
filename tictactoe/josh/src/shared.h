#include <memory>
#include <vector>

using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;
using std::weak_ptr;

struct PlayerAction
{
    int PlayerId;
    int PosX;
    int PosY;
};

class Game
{
    // for memory optimization, it could be a lookup of tiles that have non-0 values
    vector<vector<int>> m_tiles;
    bool m_isNewGame;

public:
    int WinningPlayerId;

    Game()
    {
        m_tiles = vector<vector<int>>(3, vector<int>(3, 0));
        m_isNewGame = true;
    }

    vector<vector<int>> GetTiles()
    {
        // TODO this would be somehow readonly so consumers can't arbitrarily change values
        return m_tiles;
    }

    bool IsNewGame()
    {
        return m_isNewGame;
    }

    bool Move(int playerId, int posX, int posY)
    {
        if (playerId > 0 && m_tiles[posX][posY] != 0)
        {
            return false;
        }

        m_tiles[posX][posY] = playerId;
        m_isNewGame = false;
        return true;
    }
};

class IGameService
{
public:
    virtual int GetWinnerForGameId(int gameId) = 0;
    virtual int Host() = 0;
    virtual shared_ptr<Game> Join(int gameId) = 0;
    virtual bool Move(int gameId, int playerId, int posX, int posY) = 0;
    virtual shared_ptr<PlayerAction> WaitForMove(int gameId, int playerId) = 0;
};
