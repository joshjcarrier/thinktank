#include "shared.h"
#include "server.h"

namespace TicTacToe {
namespace Client
{
    class IGameView
    {
    public:
        virtual void PromptCoordinates() = 0;
        virtual void ShowInvalidMove(int posX, int posY) = 0;
        virtual void ShowWinner(int playerId) = 0;
    };

    class IGamePresenter
    {
    public:
        virtual shared_ptr<Game> GetCurrentGame() = 0;
        virtual void HostAndJoin() = 0;
        virtual void Show() = 0;
        virtual void Move(int posX, int posY) = 0;
    };

    /// <summary>
    /// Facilitates interaction between the view and business logic.
    /// Attempt to follow Model-View-Presenter pattern.
    /// </summary>
    template<typename TView> class GamePresenter : public IGamePresenter
    {
        shared_ptr<IGameService> m_gameService;
        unique_ptr<TView> m_gameView;
        shared_ptr<Game> m_currentGame;
        int m_currentGameId;

    public:
        GamePresenter(shared_ptr<IGameService>& gameService)
        {
            m_gameService = gameService;
            shared_ptr<IGamePresenter> self(this);
            m_gameView = unique_ptr<TView>(new TView(self));
        }

        shared_ptr<Game> GetCurrentGame()
        {
            return m_currentGame;
        }

        void HostAndJoin()
        {
            m_currentGameId = m_gameService->Host();
            m_currentGame = m_gameService->Join(m_currentGameId);
        
            WaitForMove();    
            m_gameView->PromptCoordinates();
        }

        void Show()
        {
            m_gameView->ShowDialog();
        }

        void Move(int posX, int posY)
        {
            bool moveSuccess = GetCurrentGame()->Move(1, posX, posY);
            if (!moveSuccess)
            {
                m_gameView->ShowInvalidMove(posX, posY);
                m_gameView->PromptCoordinates();
                return;
            }

            int winningPlayerId = GetWinner();
            if (winningPlayerId != 0)
            {
                m_gameView->ShowWinner(winningPlayerId);
                return;
            }

            WaitForMove();
            
            winningPlayerId = GetWinner();
            if (winningPlayerId != 0)
            {
                m_gameView->ShowWinner(winningPlayerId);
                return;
            }

            m_gameView->PromptCoordinates();
        }

    private:
        int GetWinner()
        {
            return m_gameService->GetWinnerForGameId(m_currentGameId);
        }
        
        void WaitForMove()
        {
            // ideally this would block and wait until it's the players turn again
            shared_ptr<PlayerAction> opponentAction = m_gameService->WaitForMove(m_currentGameId, 1);
            m_gameService->Move(m_currentGameId, opponentAction->PlayerId, opponentAction->PosX, opponentAction->PosY);
        }
    };

    class ClientProxyGameService : public IGameService
    {
        shared_ptr<IGameService> m_gameService;
        shared_ptr<Game> m_currentGame;

    public:
        ClientProxyGameService(shared_ptr<IGameService>& gameService)
        {
            m_gameService = gameService;
        }

        static shared_ptr<IGameService> CreateLocal()
        {
            shared_ptr<IGameService> remoteGameService(InMemoryGameService::Create());
            return shared_ptr<IGameService>(new ClientProxyGameService(remoteGameService));
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
