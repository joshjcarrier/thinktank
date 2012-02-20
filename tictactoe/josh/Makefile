WX_VERSION=2.8.12

all: cli wx

cli:
	g++ src/tictactoe-cli.cpp -std=c++0x -g -o TicTacToe-cli

wx:
	g++ src/tictactoe-wx.cpp `src/wx/$(WX_VERSION)/wx-config --libs --cxxflags` -std=c++0x -g -o TicTacToe-wx

wx-init:
	cd src/wx/$(WX_VERSION)/ && chmod +x configure
	cd src/wx/$(WX_VERSION)/ && ./configure --with-x11 --disable-shared && make

clean:
	rm -f TicTacToe-*
