#include <iostream>
#include <wx/wx.h>
#include "client.h"

namespace TicTacToe {
namespace Client 
{
    // have to use regular pointers here instead of smart pointers due to integration with wxWidgets.
    class WxGameView : public IGameView, public wxFrame
    {
        shared_ptr<IGamePresenter> m_gamePresenter;
        wxButton* m_tileButtons[3][3];

    public:
        WxGameView(shared_ptr<IGamePresenter>& gamePresenter)
        : wxFrame(NULL, -1, _("Hey it's T1c t@c t0e! :) [wxWidgets edition]"), wxDefaultPosition, wxSize(450, 350))
        {
            m_gamePresenter = gamePresenter;

            wxGridSizer* gs = new wxGridSizer(3, 3, 3, 3);
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    // encode id as i * gridsize + j: determine i by dividing by gridsize, j by mod gridsize.
                    int buttonId = i * 3 + j;
                    wxButton* button =new wxButton(this, buttonId, wxT("X"));
                    m_tileButtons[i][j] = button;

                    Connect(buttonId, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(TicTacToe::Client::WxGameView::OnClick));
                    gs->Add(button, 0, wxEXPAND);
                }
            }

            SetSizer(gs);

            CreateStatusBar();
            SetStatusText(_("Ready."));
        }

        void ShowDialog()
        {
            m_gamePresenter->HostAndJoin();

            Show(true);
        }

        void PromptCoordinates()
        {
            UpdateTileButtons(false);
            SetStatusText(_("Waiting for your move..."));
        }

        void ShowInvalidMove(int posX, int posY)
        {
            SetStatusText(_("Move is invalid."));
        }

        void ShowWinner(int playerId)
        {
            UpdateTileButtons(true);

            if (playerId == 1)
            {
                SetStatusText(_("~~~~~ You win! ~~~~~"));
            }
            else
            {
                SetStatusText(_("XXXXX You lose! XXXXX"));
            }
        }

    private:
        void OnClick(wxCommandEvent& event)
        {
            event.Skip();
            SetStatusText(_("Waiting for opponent..."));

            int posX = event.GetId() / 3;
            int posY = event.GetId() - (3 * posX);
            m_gamePresenter->Move(posX, posY);
        }

        void UpdateTileButtons(bool isGameOver)
        {
            vector<vector<int>> tiles = m_gamePresenter->GetCurrentGame()->GetTiles();
            for (int i = 0; i < tiles.size(); i++)
            {
                for (int j = 0; j < tiles[0].size(); j++)
                {
                    int tilePlayerId = tiles[j][i];

                    wxString label = _("");
                    if (tilePlayerId == 1)
                    {
                        label = _("X");
                    }
                    else if (tilePlayerId == 2)
                    {
                        label = _("O");
                    }
                    else if (tilePlayerId > 2)
                    {
                        label = _("?");
                    }

                    m_tileButtons[j][i]->SetLabel(label);

                    if (isGameOver)
                    {
                        m_tileButtons[j][i]->Enable(false);
                    }
                }
            }
        }
    };

    class Application : public wxApp
    {
        unique_ptr<IGamePresenter> m_gamePresenter;
        shared_ptr<IGameService> m_gameService;

    public:
        bool OnInit()
        {
            m_gameService = shared_ptr<IGameService>(TicTacToe::Client::ClientProxyGameService::CreateLocal());
            m_gamePresenter = unique_ptr<IGamePresenter>(new GamePresenter<WxGameView>(m_gameService));
            m_gamePresenter->Show();
            return true;
        }
    };
}}

// wxWidget's bootstrapper for the main method
IMPLEMENT_APP(TicTacToe::Client::Application)
