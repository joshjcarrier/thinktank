#include <iostream>
#include "client.h"

namespace TicTacToe {
namespace Client 
{
    class WxGameView : public IGameView
    {
        shared_ptr<IGamePresenter> m_gamePresenter;

    public:
        WxGameView(shared_ptr<IGamePresenter>& gamePresenter)
        {
            m_gamePresenter = gamePresenter;
        }

        void ShowDialog()
        {
        }

        void PromptCoordinates()
        {
        }

        void ShowInvalidMove(int posX, int posY)
        {
        }

        void ShowWinner(int playerId)
        {
        }
    };


    class Application
    {
    public:
        Application()
        {
            shared_ptr<IGameService> gameService(TicTacToe::Client::ClientProxyGameService::CreateLocal());

            unique_ptr<IGamePresenter> gamePresenter(new GamePresenter<WxGameView>(gameService));
            gamePresenter->Show();
        }
    };
}}

int main()
{
    unique_ptr<TicTacToe::Client::Application> app(new TicTacToe::Client::Application());
    return 0;
}
