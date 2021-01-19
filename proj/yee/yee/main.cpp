#include <Windows.h>
#include <stdio.h>
#include <stdint.h>

#include "fucking_garbage.h"





//total-width: 912
//total-height: 583
//hor-distance to first qube: 52
//ver-distance to first qube: 95
//board-width: 810
//board-height: 433
//sqube-side: 27

struct vec2 {
	float x, y;
};

vec2 getloc(uint32_t x, uint32_t y, float width, float height) {
	vec2 topleft_corner = { (52.f / 912.f)*width, (95.f / 583.f)*height };
	float side_to_scale = 27.f*width / 912.f;

	return {
		topleft_corner.x + side_to_scale * x + side_to_scale / 2.f,
		topleft_corner.y + side_to_scale * y + side_to_scale / 2.f
	};
}








HANDLE process = 0;

uint8_t* deref(uint8_t* addr) {
	uint8_t* result = 0; SIZE_T o_o;
	ReadProcessMemory(process, addr, &result, sizeof(uint8_t*), &o_o);
	return result;
}

int main() {

	HWND mnsw_window = FindWindowA(0, "Minesweeper");

	DWORD process_id = 0;
	GetWindowThreadProcessId(mnsw_window, &process_id);
	process = OpenProcess(PROCESS_ALL_ACCESS, 0, process_id);
	uint8_t* base = (uint8_t*)GetModule("Minesweeper.exe", process);

	uint8_t* mnsw_board = deref(deref(base + 0x000AAA38)+0x18);
	
	uint8_t board[30][16] = {};
	uint8_t* arrays_for_columns = deref(deref(mnsw_board+0x58) + 0x10);

	for (size_t i = 0; i < 30; i++){
		uint8_t* column = deref(deref(arrays_for_columns + 0x8 * i) + 0x10);
		ReadProcessMemory(process, column, board[i], 16, 0);
		//char shitthing[16] = {}; memset(shitthing, 0, 16);
		//if (i == 1) { shitthing[1] = 1; shitthing[2] = 1; shitthing[3] = 1;}
		//if (i == 2) { shitthing[1] = 1; shitthing[2] = 0; shitthing[3] = 1; }
		//if (i == 3) { shitthing[1] = 1; shitthing[2] = 1; shitthing[3] = 1; }
		//WriteProcessMemory(process, column, shitthing, 16, 0);
	}

	for (size_t y = 0; y < 16; y++){
		for (size_t x = 0; x < 30; x++){
			printf("%c", board[x][y]?'X':'+');
		}
		printf("\n");
	}

	RECT window_rect = {};
	if (!GetWindowRect(mnsw_window, &window_rect))printf("FUCK\n");

	uint32_t
		wx = window_rect.left,
		wy = window_rect.top,
		width = window_rect.right - window_rect.left,
		height = window_rect.bottom - window_rect.top
	;
	
	


	vec2 clickpos = getloc(5, 1, width, height);
	printf("moving to %d, %d\n", LONG(wx + clickpos.x), LONG(wy + clickpos.y));
	SetCursorPos(LONG(wx + clickpos.x), LONG(wy + clickpos.y));
	

	INPUT hah[2] = {
		{ INPUT_MOUSE, MOUSEINPUT{0, 0, 0, MOUSEEVENTF_LEFTDOWN, 0} },
		{ INPUT_MOUSE, MOUSEINPUT{0, 0, 0, MOUSEEVENTF_LEFTUP,   0} }
	};
	SendInput(2, &hah[0], sizeof(INPUT));


	for (size_t y = 0; y < 16; y++) {
		for (size_t x = 0; x < 30; x++) {
			vec2 clickpos = getloc(x, y, width, height);
			printf("moving to %d, %d\n", LONG(wx + clickpos.x), LONG(wy + clickpos.y));
			SetCursorPos(LONG(wx + clickpos.x), LONG(wy + clickpos.y));
			if (board[x][y]) {
				INPUT heh[2] = {
					{ INPUT_MOUSE, MOUSEINPUT{0, 0, 0, MOUSEEVENTF_RIGHTDOWN, 0} },
					{ INPUT_MOUSE, MOUSEINPUT{0, 0, 0, MOUSEEVENTF_RIGHTUP,   0} }
				};
				SendInput(2, &heh[0], sizeof(INPUT));
			} else {
				INPUT heh[2] = {
					{ INPUT_MOUSE, MOUSEINPUT{0, 0, 0, MOUSEEVENTF_LEFTDOWN, 0} },
					{ INPUT_MOUSE, MOUSEINPUT{0, 0, 0, MOUSEEVENTF_LEFTUP,   0} }
				};
				SendInput(2, &heh[0], sizeof(INPUT));
			}
			Sleep(10);
		}
	}
	
	

	getchar();

	return 0;
}
