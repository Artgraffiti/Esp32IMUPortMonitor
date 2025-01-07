#include <cstddef>
#include <iostream>
#include <cstring>

#include "window.h"
#include "text.h"
#include "list.h"
#include "image.h"
#include "button.h"
#include "GUI.h"

#define H_RGB(hex)                                \
    RGB((double)(((hex) & 0xff0000) >> 16) / 256, \
        (double)(((hex) & 0x00ff00) >> 8) / 256,  \
        (double)(((hex) & 0x0000ff)) / 256)

#define WIN_WIDTH 1200
#define WIN_HEIGHT 800
#define WIN_BCK_COLOR H_RGB(0x0D1117)
#define BTNS_COLOR H_RGB(0x010409)
#define BTN_PORT_MONITOR_WIDTH WIN_WIDTH / 2
#define BTN_PORT_MONITOR_HEIGHT 60

enum UserEventType {
    EVENT_BTN_PORT_MONITOR,
    EVENT_BTN_VISUALIZATION,
};

class MainWindow : public Window {
   public:
    MainWindow() { m_ClassName = __FUNCTION__; }
    ~MainWindow() {}

    void OnCreate();
    void OnDraw(Context *cr);
    bool OnKeyPress(uint64_t value);
    void OnNotify(Window *child, uint32_t type, const Point &position);
    void OnSizeChanged();

   private:
    TextButton *m_pBtnPortMonitor, *m_pBtnVisualization;
};

void MainWindow::OnCreate() {
    std::cout << "MainWindow::OnCreate()" << std::endl;

    TextButton *pBtn;
    
    pBtn = new TextButton("Монитор порта", EVENT_BTN_PORT_MONITOR);
    AddChild(pBtn, Point(0, 0), Rect(BTN_PORT_MONITOR_WIDTH, BTN_PORT_MONITOR_HEIGHT));
    pBtn->SetBackColor(BTNS_COLOR);
    pBtn->SetDarkColor(BTNS_COLOR);
    pBtn->SetLiteColor(BTNS_COLOR);
    pBtn->SetFrameColor(H_RGB(0x31363D));
    pBtn->SetFontSize(26);
    pBtn->SetTextColor(H_RGB(0xffffff));

    pBtn = new TextButton("Визуализация", EVENT_BTN_VISUALIZATION);
    AddChild(pBtn, Point(BTN_PORT_MONITOR_WIDTH, 0), Rect(BTN_PORT_MONITOR_WIDTH, BTN_PORT_MONITOR_HEIGHT));
    pBtn->SetBackColor(BTNS_COLOR);
    pBtn->SetDarkColor(BTNS_COLOR);
    pBtn->SetLiteColor(BTNS_COLOR);
    pBtn->SetFrameColor(H_RGB(0x31363D));
    pBtn->SetFontSize(26);
    pBtn->SetTextColor(H_RGB(0xffffff));

    CaptureKeyboard(this);
}

void MainWindow::OnDraw(Context *cr) {
    std::cout << "MainWindow::OnDraw()" << std::endl;
    Point wsize = GetInteriorSize();

    cr->SetColor(WIN_BCK_COLOR);
    cr->FillRectangle(Point(0, 0), wsize);
}

void MainWindow::OnNotify(Window *child, uint32_t type, const Point &position) {
    std::cout << "MainWindow::OnNotify()" << std::endl;

    switch (type) {
        case EVENT_BTN_PORT_MONITOR:
        case EVENT_BTN_VISUALIZATION:
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

int main(int argc, char **argv) {
    MainWindow *pWindow = new MainWindow;

    int res = Run(argc, argv, pWindow, WIN_WIDTH, WIN_HEIGHT);
    delete pWindow;
    return res;
}
