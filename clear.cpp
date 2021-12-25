#include <Windows.h>

void clear(HDC hdc)
{
    Sleep(1500);
    HPEN hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    SelectObject(hdc, hpen);

    HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, hbrush);

    Rectangle(hdc, 0, 0, 800, 800);
}
