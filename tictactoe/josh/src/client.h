#include "shared.h"
#include "server.h"

namespace TicTacToe {
namespace Client
{
    class ClientProxyGameService : public IGameService
    {
        IGameService* m_gameService;
        Game* m_currentGame;

    public:
        ClientProxyGameService(IGameService* gameService)
        {
            m_gameService = gameService;
        }

        static ClientProxyGameService* CreateLocal()
        {
            return new ClientProxyGameService(InMemoryGameService::Create());
        }

        int GetWinnerForGameId(int gameId)
        {
            return m_gameService->GetWinnerForGameId(gameId);
        }

        Game* Join(int gameId)
        {
            m_currentGame = m_gameService->Join(gameId);
            return m_currentGame;
        }

        int Host()
        {
            return m_gameService->Host();
        }

        bool Move(int gameId, int playerId, int posX, int posY)
        {
            return m_gameService->Move(gameId, playerId, posX, posY);
        }

        PlayerAction* WaitForMove(int gameId, int playerId)
        {
            return m_gameService->WaitForMove(gameId, playerId);
        }
    };
}}
