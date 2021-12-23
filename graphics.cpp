#include <vector>
#include <Windows.h>

void drawGraphics(HDC hDC, std::vector<double>& vec) {

    HPEN Pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    SelectObject(hDC, Pen);

    MoveToEx(hDC, 20, 20, NULL);
    LineTo(hDC, 20, 270);
    MoveToEx(hDC, 20, 270, NULL);
    LineTo(hDC, 550, 270);
    int x;

    for (int i = 1; i <= 10; i++) {

        MoveToEx(hDC, 23, 250 - i * 25, NULL);
        LineTo(hDC, 17, 250 - i * 25);

        MoveToEx(hDC, 20 + 50 * i, 273, NULL);
        LineTo(hDC, 20 + 50 * i, 267);

    }

    MoveToEx(hDC, 20, 270, NULL);

    for (int i = 0; i < vec.size(); i++) {
        LineTo(hDC, 25 * (i + 1), 270 - vec[i]);
        MoveToEx(hDC, 25 * (i + 1), 270 - vec[i], NULL);


    }

}