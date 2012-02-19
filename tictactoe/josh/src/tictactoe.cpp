#include <iostream>
#include "client.h"

namespace TicTacToe {
namespace Client 
{

    class IGamePresenter
    {
    public:
        virtual Game* GetCurrentGame() = 0;
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
       IGamePresenter* m_gamePresenter;

    public:
        GameView(IGamePresenter* gamePresenter)
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

        int PromptInteger(std::string prompt, int min, int max)
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
            int*** tiles = m_gamePresenter->GetCurrentGame()->GetTiles();
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    int tilePlayerId = *tiles[j][i];

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

                    if (j < 3-1)
                    {
                        std:: cout << " |";
                    }
                }

                std::cout << std::endl;

                if (i < 3-1)
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
        IGameService* m_gameService;
        GameView* m_gameView;
        Game* m_currentGame;
        int m_currentGameId;

    public:
        GamePresenter(IGameService* gameService)
        {
            m_gameService = gameService;
            m_gameView = new GameView(this);
        }

        Game* GetCurrentGame()
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
            PlayerAction* opponentAction = m_gameService->WaitForMove(m_currentGameId, 1);
            m_gameService->Move(m_currentGameId, opponentAction->PlayerId, opponentAction->PosX, opponentAction->PosY);
        }
    };

    class Application
    {
    public:
        Application()
        {
            IGameService* gameService = TicTacToe::Client::ClientProxyGameService::CreateLocal();

            GamePresenter* gamePresenter = new GamePresenter(gameService);
            gamePresenter->Show();
        }
    };
}}

int main()
{
    TicTacToe::Client::Application* application = new TicTacToe::Client::Application();
    std::cout << "kthxbai" << std::endl;
    return 0;
}
