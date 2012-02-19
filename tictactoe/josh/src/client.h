#include "shared.h"
#include "server.h"

namespace TicTacToe {
namespace Client
{
    class ClientProxyGameService : public IGameService
    {
        shared_ptr<IGameService> m_gameService;
        shared_ptr<Game> m_currentGame;

    public:
        ClientProxyGameService(shared_ptr<IGameService>& gameService)
        {
            m_gameService = gameService;
        }

        static shared_ptr<ClientProxyGameService> CreateLocal()
        {
            auto remoteGameService = shared_ptr<IGameService>(InMemoryGameService::Create());
            auto instance = shared_ptr<ClientProxyGameService>(new ClientProxyGameService(remoteGameService));
            return instance;
        }

        int GetWinnerForGameId(int gameId)
        {
            return m_gameService->GetWinnerForGameId(gameId);
        }

        shared_ptr<Game> Join(int gameId)
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

        shared_ptr<PlayerAction> WaitForMove(int gameId, int playerId)
        {
            return m_gameService->WaitForMove(gameId, playerId);
        }
    };
}}
