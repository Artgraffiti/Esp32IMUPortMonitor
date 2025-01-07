#include <iostream>
#include <cstring>

#include "window.h"
#include "text.h"
#include "image.h"
#include "button.h"
#include "GUI.h"

#define H_RGB(hex)                                \
    RGB((double)(((hex) & 0xff0000) >> 16) / 256, \
        (double)(((hex) & 0x00ff00) >> 8) / 256,  \
        (double)(((hex) & 0x0000ff)) / 256)

#define WIN_WIDTH 1200
#define WIN_HEIGHT 800
#define OUT_FIELD_WIDTH 300
#define OUT_FIELD_HEIGHT 50
#define BTN_CALCULATE_WIDTH OUT_FIELD_WIDTH
#define BTN_CALCULATE_HEIGHT 50

enum UserEventType {
    EVENT_0,
    EVENT_1,
};

class MainWindow : public Window {
   public:
    MainWindow() {}
    ~MainWindow() {}

    void OnCreate();
    void OnDraw(Context *cr);
    bool OnKeyPress(uint64_t value);
    void OnNotify(Window *child, uint32_t type, const Point &position);
    void OnSizeChanged();

    void ProcessDigit(uint16_t digit);
    void PrintString();

   private:
    char o_field[80];
};

void MainWindow::OnCreate() {
    std::cout << "MainWindow::OnCreate()" << std::endl;

    AddChild(new TextButton("Расчитать", EVENT_0),
             Point(WIN_WIDTH / 2 - (BTN_CALCULATE_WIDTH / 2),
                   WIN_HEIGHT / 2 - (BTN_CALCULATE_HEIGHT / 2)),
             Rect(BTN_CALCULATE_WIDTH, BTN_CALCULATE_HEIGHT));

    CaptureKeyboard(this);
}

void MainWindow::OnDraw(Context *cr) {
    std::cout << "MainWindow::OnDraw()" << std::endl;
    Point wsize = GetInteriorSize();

    cr->SetColor(H_RGB(0x0e1a4f));
    cr->FillRectangle(Point(0, 0), wsize);
}

void MainWindow::OnNotify(Window *child, uint32_t type, const Point &position) {
    std::cout << "MainWindow::OnNotify()" << std::endl;

    switch (type) {
        case EVENT_0:

        default:;
    }
}

bool MainWindow::OnKeyPress(uint64_t keyval) {
    switch (keyval) {
        case 'q':
            DeleteMe();
            return true;
        default:;
    }
    return true;
}

void MainWindow::OnSizeChanged() {}

void ProcessDigit(uint16_t digit) {}

int main(int argc, char **argv) {
    MainWindow *pWindow = new MainWindow;

    int res = Run(argc, argv, pWindow, WIN_WIDTH, WIN_HEIGHT);
    delete pWindow;
    return res;
}
