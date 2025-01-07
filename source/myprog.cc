#include <cstdint>
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
#define BTNS_FRAME_COLOR H_RGB(0x31363D)
#define BTNS_HEIGHT 60

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
    
    Rect window_size =  this->GetInteriorSize();
    uint16_t btns_width = window_size.GetWidth() / 2;
    uint16_t btns_height = BTNS_HEIGHT;
    m_pBtnPortMonitor = new TextButton("Монитор порта", EVENT_BTN_PORT_MONITOR);
    AddChild(m_pBtnPortMonitor, Point(0, 0), Rect(btns_width, btns_height));
    m_pBtnPortMonitor->SetBackColor(BTNS_COLOR);
    m_pBtnPortMonitor->SetDarkColor(BTNS_COLOR);
    m_pBtnPortMonitor->SetLiteColor(BTNS_COLOR);
    m_pBtnPortMonitor->SetFrameColor(BTNS_FRAME_COLOR);
    m_pBtnPortMonitor->SetFontSize(26);
    m_pBtnPortMonitor->SetTextColor(H_RGB(0xffffff));

    m_pBtnVisualization = new TextButton("Визуализация", EVENT_BTN_VISUALIZATION);
    AddChild(m_pBtnVisualization, Point(btns_width, 0), Rect(btns_width, btns_height));
    m_pBtnVisualization->SetBackColor(BTNS_COLOR);
    m_pBtnVisualization->SetDarkColor(BTNS_COLOR);
    m_pBtnVisualization->SetLiteColor(BTNS_COLOR);
    m_pBtnVisualization->SetFrameColor(BTNS_FRAME_COLOR);
    m_pBtnVisualization->SetFontSize(26);
    m_pBtnVisualization->SetTextColor(H_RGB(0xffffff));

    CaptureKeyboard(this);
}

void MainWindow::OnSizeChanged() {
    Rect window_size =  this->GetInteriorSize();
    uint16_t btns_width = window_size.GetWidth() / 2;
    uint16_t btns_height = BTNS_HEIGHT;

    m_pBtnPortMonitor->SetSize(Rect(btns_width, btns_height));
    m_pBtnPortMonitor->SetPosition(Point(0, 0));
    m_pBtnPortMonitor->ReDraw();

    m_pBtnVisualization->SetSize(Rect(btns_width, btns_height));
    m_pBtnVisualization->SetPosition(Point(btns_width, 0));
    m_pBtnVisualization->SetBackColor(BTNS_COLOR);
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

int main(int argc, char **argv) {
    MainWindow *pWindow = new MainWindow;

    int res = Run(argc, argv, pWindow, WIN_WIDTH, WIN_HEIGHT);
    delete pWindow;
    return res;
}
