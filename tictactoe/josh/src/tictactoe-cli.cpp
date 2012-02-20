#include <iostream>
#include "client.h"

using std::cin;
using std::cout;
using std::endl;

namespace TicTacToe {
namespace Client 
{
    /// <summary>
    /// Ideally this view can be replaced by any UX medium: console, xterm, etc.
    /// </summary>
    class ConsoleGameView : public IGameView
    {
        shared_ptr<IGamePresenter> m_gamePresenter;

    public:
        ConsoleGameView(shared_ptr<IGamePresenter>& gamePresenter)
        {
            m_gamePresenter = gamePresenter;
        }

        void ShowDialog()
        {
            cout << "hey it's T1c t@c t0e! :)    [console edition]" << endl;
            cout << endl;
            cout << "You're 'X'." << endl;

            m_gamePresenter->HostAndJoin();
        }

        void PromptCoordinates()
        {
            cout << endl;
            ShowTiles();
            cout << endl;

            int xCoordChoice = PromptInteger("Next move X co-ordinate? (1-3)", 1, 3);
            int yCoordChoice = PromptInteger("Next move Y co-ordinate? (1-3)", 1, 3);

            m_gamePresenter->Move(xCoordChoice - 1, yCoordChoice - 1);
        }

        void ShowInvalidMove(int posX, int posY)
        {
            cout << "Move is invalid." << endl;
        }

        void ShowWinner(int playerId)
        {
            cout << endl;
            ShowTiles();
            cout << endl;

            if (playerId == 1)
            {
                cout << "You win!" << endl;
            }
            else
            {
                cout << "You lose!" << endl;
            }

            cout << "kthxbai" << endl;
        }

    private:
        int PromptInteger(string const& prompt, int min, int max)
        {
            bool isValid = false;

            int inputInt;
            while (!isValid)
            {
                cout << prompt << " ";
                cin >> inputInt;

                if (cin.fail() || inputInt < min || inputInt > max)
                {
                    cin.clear();
                    cin.ignore();
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

                    cout << " ";
                    if (tilePlayerId == 0)
                    {
                        cout << " ";
                    }
                    else if (tilePlayerId == 1)
                    {
                        cout << "X";
                    }
                    else if (tilePlayerId == 2)
                    {
                        cout << "O";
                    }
                    else
                    {
                        cout << tilePlayerId;
                    }

                    if (j < tiles[0].size() - 1)
                    {
                        cout << " |";
                    }
                }

                cout << endl;

                if (i < tiles.size() - 1)
                {
                    cout << "-----------" << std::endl;
                }
            }
        }
    };


    class Application
    {
    public:
        Application()
        {
            auto gameService = shared_ptr<IGameService>(TicTacToe::Client::ClientProxyGameService::CreateLocal());

            auto gamePresenter = unique_ptr<IGamePresenter>(new GamePresenter<ConsoleGameView>(gameService));
            gamePresenter->Show();
        }
    };
}}

int main()
{
    auto application = unique_ptr<TicTacToe::Client::Application>(new TicTacToe::Client::Application());
    return 0;
}
