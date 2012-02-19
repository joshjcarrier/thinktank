struct PlayerAction
{
    int PlayerId;
    int PosX;
    int PosY;
};

class Game
{
    int*** tiles;

public:
    int WinningPlayerId;

    Game()
    {
        tiles = new int**[3];
        for (int i = 0; i < 3; i++)
        {
            tiles[i] = new int*[3];

            for (int j = 0; j < 3; j++)
            {
                tiles[i][j] = new int[3];
            }
        }
    }

    ~Game()
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                delete [] tiles[i][j];
            }

            delete [] tiles[i];
        }
    }

    int*** GetTiles()
    {
        return tiles;
    }

    bool Move(int playerId, int posX, int posY)
    {
        if (playerId > 0 && *tiles[posX][posY] != 0)
        {
            return false;
        }

        *tiles[posX][posY] = playerId;
        return true;
    }
};

class IGameService
{
public:
    virtual int GetWinnerForGameId(int gameId) = 0;
    virtual int Host() = 0;
    virtual Game* Join(int gameId) = 0;
    virtual bool Move(int gameId, int playerId, int posX, int posY) = 0;
    virtual PlayerAction* WaitForMove(int gameId, int playerId) = 0;
};
