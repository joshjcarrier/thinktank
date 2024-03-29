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
        weak_ptr<IGamePresenter> m_gamePresenter;

    public:
        ConsoleGameView(weak_ptr<IGamePresenter>& gamePresenter)
        {
            m_gamePresenter = gamePresenter;
        }

        void ShowDialog()
        {
            cout << "hey it's T1c t@c t0e! :)    [console edition]" << endl;
            cout << endl;
            cout << "You're 'X'." << endl;

            m_gamePresenter.lock()->HostAndJoin();
        }

        void PromptCoordinates()
        {
            cout << endl;
            ShowTiles();
            cout << endl;

            int xCoordChoice = PromptInteger("Next move X co-ordinate? (1-3)", 1, 3);
            int yCoordChoice = PromptInteger("Next move Y co-ordinate? (1-3)", 1, 3);

            m_gamePresenter.lock()->Move(xCoordChoice - 1, yCoordChoice - 1);
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
            vector<vector<int>> tiles = m_gamePresenter.lock()->GetCurrentGame()->GetTiles();
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
            shared_ptr<IGameService> gameService(TicTacToe::Client::ClientProxyGameService::CreateLocal());

            shared_ptr<IGamePresenter> gamePresenter(new GamePresenter<ConsoleGameView>(gameService));
            weak_ptr<IGamePresenter> weakGamePresenter(gamePresenter);
            gamePresenter->Initialize(weakGamePresenter);
            gamePresenter->Show();
        }
    };
}}

int main()
{
    unique_ptr<TicTacToe::Client::Application> app(new TicTacToe::Client::Application());
    return 0;
}
