#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
#include "tetrisMap.h"
#include "ui.h"

using namespace std::chrono_literals;

class FlushScreen {
	HANDLE cmd_output_handle = nullptr;
public:
	FlushScreen() {
		CONSOLE_CURSOR_INFO cursorInfo{ 1, FALSE };       // Get more details on MSDN .
		cmd_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleCursorInfo(cmd_output_handle, &cursorInfo);
	}

	~FlushScreen() {
		if (cmd_output_handle != nullptr)
			CloseHandle(cmd_output_handle);
	}

	void operator()() noexcept {
		SetConsoleCursorPosition(cmd_output_handle, COORD{ 0,0 });
	}
};

int main() {
	system("color 0A");
	TetrisMap<16, 26, 7, 3> map;
	
	std::jthread inputHandler{ [&map]() {
		auto pressed = [](int vkey) {                       // virtual key: see https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
				return GetAsyncKeyState(vkey) & 0x0001;     // GetAsyncKeyState(): see https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getasynckeystate
			};

		while(!map.isGameOver()) {
			std::this_thread::sleep_for(10ms);

			if (pressed(VK_UP)) {
				map.rotate();
			}
			else if (pressed(VK_DOWN)) {
				map.goDown();
			}	
			else if (pressed(VK_LEFT)) {
				map.goLeft();
			}	
			else if (pressed(VK_RIGHT)) {
				map.goRight();
			}
		}
	} };

	std::jthread everyHalfSecondDownBlock { [&map]() {
		while (!map.isGameOver()) {
			std::this_thread::sleep_for(500ms);
			map.goDown();
		}
	} };

	std::jthread drawGame{ [&map]() {
		FlushScreen fs{};

		while (!map.isGameOver()) {
			std::this_thread::sleep_for(50ms);

			fs();
			drawTetrisMap(map);
		}
	} };
}
