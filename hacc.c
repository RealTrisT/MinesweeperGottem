#include <Windows.h>
#include <stdio.h>
#include <stdint.h>



BOOL WINAPI GetScreenPiece(WCHAR *wPath, uint32_t xx, uint32_t yy, uint32_t width, uint32_t height, char* buffer)
{
    //BITMAPFILEHEADER bfHeader;
    BITMAPINFOHEADER biHeader;
    BITMAPINFO bInfo;
    HGDIOBJ hTempBitmap;
    HBITMAP hBitmap;
    BITMAP bAllDesktops;
    HDC hDC, hMemDC;
    LONG lWidth, lHeight;
    BYTE *bBits = NULL;
    HANDLE hHeap = GetProcessHeap();
    DWORD cbBits, dwWritten = 0;
    //HANDLE hFile;
    INT x = GetSystemMetrics(SM_XVIRTUALSCREEN);
    INT y = GetSystemMetrics(SM_YVIRTUALSCREEN);

    //ZeroMemory(&bfHeader, sizeof(BITMAPFILEHEADER));
    ZeroMemory(&biHeader, sizeof(BITMAPINFOHEADER));
    ZeroMemory(&bInfo, sizeof(BITMAPINFO));
    ZeroMemory(&bAllDesktops, sizeof(BITMAP));

    hDC = GetDC(NULL);
    hTempBitmap = GetCurrentObject(hDC, OBJ_BITMAP);
    GetObjectW(hTempBitmap, sizeof(BITMAP), &bAllDesktops);

    lWidth = width;//bAllDesktops.bmWidth;
    lHeight = height;//bAllDesktops.bmHeight;

    DeleteObject(hTempBitmap);

    //bfHeader.bfType = (WORD)('B' | ('M' << 8));
    //bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    biHeader.biSize = sizeof(BITMAPINFOHEADER);
    biHeader.biBitCount = 24;
    biHeader.biCompression = BI_RGB;
    biHeader.biPlanes = 1;
    biHeader.biWidth = lWidth;
    biHeader.biHeight = lHeight;

    bInfo.bmiHeader = biHeader;

    cbBits = (((24 * lWidth + 31)&~31) / 8) * lHeight;

    hMemDC = CreateCompatibleDC(hDC);
    hBitmap = CreateDIBSection(hDC, &bInfo, DIB_RGB_COLORS, (VOID **)&bBits, NULL, 0);
    SelectObject(hMemDC, hBitmap);
    BitBlt(hMemDC, 0, 0, width, height, hDC, xx, yy, SRCCOPY);
    
    //printf("%d, %d\n", x, y);

    memcpy(buffer, bBits, cbBits);

    //hFile = CreateFileW(wPath, GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    //WriteFile(hFile, &bfHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
    //WriteFile(hFile, &biHeader, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
    //WriteFile(hFile, bBits, cbBits, &dwWritten, NULL);
    //CloseHandle(hFile);

    DeleteDC(hMemDC);
    ReleaseDC(NULL, hDC);
    DeleteObject(hBitmap);

    return TRUE;
}









typedef struct {
	uint32_t x, y;
} vec2;






char* buffer = 0;
HWND mnsw_window = 0;

const uint32_t tile_width = 30;
const uint32_t tile_height = 16;

char* board = 0;		//-1 unset;    0 : empty;     1-8 : the respective values;

vec2 topleft_pix = {38, 81};


typedef struct{
	char b, g, r;
}__attribute__((packed)) rgb;

rgb* getpixel(uint32_t x, uint32_t y){
	return (rgb*)&buffer[row_size*(height-1-y) + x*3];
}

char getCell(uint32_t x, uint32_t y){
	vec2 base_pix = {38 + 18*x}
}

int main(int argc, char const *argv[]){


	mnsw_window = FindWindowA(0, "Minesweeper");
	board = malloc(tile_height*tile_width);

	DWORD process_id = 0;
	GetWindowThreadProcessId(mnsw_window, &process);
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, 0, process_id);
	//ReadProcessMemory()


	for (;;){
		RECT window_rect = {};
		if(!GetWindowRect(mnsw_window, &window_rect))break;

		uint32_t 
			x = window_rect.left, 
			y = window_rect.top, 
			width = window_rect.right - window_rect.left, 
			height = window_rect.bottom - window_rect.top
		;


		//GetScreenPiece(L"gottem.bmp", x, y, width, height, buffer);



	}

	printf("something went wrong\n");

	



	uint32_t row_size = (3*width+3)&~3;
	#define getpix(x, y, buffer) (&((char*)buffer)[row_size*(height-1-y) + x*3])

	printf("(%d, %d) %dx%d\n", window_rect.left, window_rect.top, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top);

	buffer = malloc(height*row_size);
	GetScreenPiece(L"gottem.bmp", x, y, width, height, buffer);
	printf("%u, %u, %u\n", getpix(38, 81, buffer)[0], getpix(38, 81, buffer)[1], getpix(38, 81, buffer)[2]);

	//SaveBitmap(L"gottem.bmp");
	return 0;
}

//38, 81 : top left corner
//18x18 : each cell

//http://www.minesweeper.info/wiki/Strategy

