#include <iostream>
#include "client.h"

namespace TicTacToe {
namespace Client 
{

    class IGamePresenter
    {
    public:
        virtual shared_ptr<Game> GetCurrentGame() = 0;
        virtual int GetWinner() = 0;
        virtual void HostAndJoin() = 0;
        virtual void Show() = 0;
        virtual bool Move(int posX, int posY) = 0;
        virtual void WaitForMove() = 0;
    };

    /// <summary>
    /// Ideally this view can be replaced by any UX medium: console, xterm, etc.
    /// </summary>
    class GameView
    {
       shared_ptr<IGamePresenter> m_gamePresenter;

    public:
        GameView(shared_ptr<IGamePresenter>& gamePresenter)
        {
            m_gamePresenter = gamePresenter;
        }

        void ShowDialog()
        {
            std::cout << "hey it's T1c t@c t0e! :)" << std::endl;

            m_gamePresenter->HostAndJoin();

            int winningPlayerId = 0;            
            while (winningPlayerId == 0)
            {
                PromptCoordinates();

                // determine if got the winning move
                winningPlayerId = m_gamePresenter->GetWinner();
                if (winningPlayerId != 0)
                {
                    break;
                }

                m_gamePresenter->WaitForMove();

                winningPlayerId = m_gamePresenter->GetWinner();
            }

            std::cout << std::endl;
            ShowTiles();
            std::cout << std::endl;

            if (winningPlayerId == 1)
            {
                std::cout << "You win!" << std::endl;
            }
            else
            {
                std::cout << "You lose!" << std::endl;
            }
        }

    private:
        void PromptCoordinates()
        {
            bool isValidMove = false;
            while (!isValidMove)
            {
                std::cout << std::endl;
                ShowTiles();
                std::cout << std::endl;

                int xCoordChoice = PromptInteger("Next move X co-ordinate? (1-3)", 1, 3);
                int yCoordChoice = PromptInteger("Next move Y co-ordinate? (1-3)", 1, 3);

                isValidMove = m_gamePresenter->Move(xCoordChoice - 1, yCoordChoice - 1);
                if (!isValidMove)
                {
                    std::cout << "Move is invalid." << std::endl;
                }
            }
        }

        int PromptInteger(string const& prompt, int min, int max)
        {
            bool isValid = false;

            int inputInt;
            while (!isValid)
            {
                std::cout << prompt << " ";
                std::cin >> inputInt;

                if (std::cin.fail() || inputInt < min || inputInt > max)
                {
                    std::cin.clear();
                    std::cin.ignore();
                    continue;
                }

                isValid = true;
            }

            return inputInt;
        }

        void ShowTiles()
        {
            auto tiles = m_gamePresenter->GetCurrentGame()->GetTiles();
            for (int i = 0; i < tiles.size(); i++)
            {
                for (int j = 0; j < tiles[0].size(); j++)
                {
                    int tilePlayerId = tiles[j][i];

                    std::cout << " ";
                    if (tilePlayerId == 0)
                    {
                        std::cout << " ";
                    }
                    else if (tilePlayerId == 1)
                    {
                        std::cout << "X";
                    }
                    else if (tilePlayerId == 2)
                    {
                        std::cout << "O";
                    }
                    else
                    {
                        std::cout << tilePlayerId;
                    }

                    if (j < tiles[0].size() - 1)
                    {
                        std:: cout << " |";
                    }
                }

                std::cout << std::endl;

                if (i < tiles.size() - 1)
                {
                    std::cout << "-----------" << std::endl;
                }
            }
        }
    };

    /// <summary>
    /// Facilitates interaction between the view and business logic.
    /// Attempt to follow Model-View-Presenter pattern.
    /// </summary>
    class GamePresenter : public IGamePresenter
    {
        shared_ptr<IGameService> m_gameService;
        unique_ptr<GameView> m_gameView;
        shared_ptr<Game> m_currentGame;
        int m_currentGameId;

    public:
        GamePresenter(shared_ptr<IGameService>& gameService)
        {
            m_gameService = gameService;
            auto self = shared_ptr<IGamePresenter>(this);
            m_gameView = unique_ptr<GameView>(new GameView(self));
        }

        shared_ptr<Game> GetCurrentGame()
        {
            return m_currentGame;
        }

        int GetWinner()
        {
            return m_gameService->GetWinnerForGameId(m_currentGameId);
        }

        void HostAndJoin()
        {
            m_currentGameId = m_gameService->Host();
            m_currentGame = m_gameService->Join(m_currentGameId);
        }

        void Show()
        {
            m_gameView->ShowDialog();
        }

        bool Move(int posX, int posY)
        {
            return GetCurrentGame()->Move(1, posX, posY);
        }

        void WaitForMove()
        {
            // ideally this would block and wait until it's the players turn again
            auto opponentAction = m_gameService->WaitForMove(m_currentGameId, 1);
            m_gameService->Move(m_currentGameId, opponentAction->PlayerId, opponentAction->PosX, opponentAction->PosY);
        }
    };

    class Application
    {
    public:
        Application()
        {
            auto gameService = shared_ptr<IGameService>(TicTacToe::Client::ClientProxyGameService::CreateLocal());

            auto gamePresenter = unique_ptr<IGamePresenter>(new GamePresenter(gameService));
            gamePresenter->Show();
        }
    };
}}

int main()
{
    auto application = unique_ptr<TicTacToe::Client::Application>(new TicTacToe::Client::Application());
    std::cout << "kthxbai" << std::endl;
    return 0;
}
