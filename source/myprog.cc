#include <cstdint>
#include <cstdio>
#include <iostream>
#include <cstring>

#include "port_monitor.h"

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
#define PM_LINE_HEIGHT 20

enum UserEventType {
    EVENT_BTN_PORT_MONITOR = 2,
    EVENT_BTN_VISUALIZATION,
};

class MainWindow : public Window {
   public:
    MainWindow() { m_ClassName = __FUNCTION__; }

    void OnCreate();
    void OnDraw(Context *cr);
    bool OnKeyPress(uint64_t value);
    void OnNotify(Window *child, uint32_t type, const Point &position);

   private:
    TextButton *m_pBtnPortMonitor, *m_pBtnVisualization;
    Scroll *m_pSclPortMonitor;
    PortMonitor *m_pLstPortMonitor;
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

    m_pBtnVisualization =
        new TextButton("Визуализация", EVENT_BTN_VISUALIZATION);
    AddChild(m_pBtnVisualization, Point(btns_width, 0),
             Rect(btns_width, btns_height));
    m_pBtnVisualization->SetBackColor(BTNS_COLOR);
    m_pBtnVisualization->SetDarkColor(BTNS_COLOR);
    m_pBtnVisualization->SetLiteColor(BTNS_COLOR);
    m_pBtnVisualization->SetFrameColor(BTNS_FRAME_COLOR);
    m_pBtnVisualization->SetFontSize(26);
    m_pBtnVisualization->SetTextColor(H_RGB(0xffffff));

    m_pSclPortMonitor = new Scroll;
    AddChild(m_pSclPortMonitor, Point(0, btns_height), Rect(WIN_WIDTH, WIN_HEIGHT - btns_height));
    m_pLstPortMonitor = new PortMonitor();
    m_pLstPortMonitor->SetElementHeight(PM_LINE_HEIGHT);
    m_pSclPortMonitor->SetDataWindow(m_pLstPortMonitor);

#if 0
    for (int i = 0; i < 1000; i++) {
        char buff[20];
        sprintf(buff, "Hello %d", i);
        Text *pTxt = new Text(buff);
        pTxt->SetFont(0, 16, -1, -1);
        pTxt->SetAlignment(TEXT_ALIGNV_MASK);
        m_pLstPortMonitor->Insert(0, pTxt);
    }
#endif

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
            m_pLstPortMonitor->Start();
            break;
        case EVENT_BTN_VISUALIZATION:
            m_pLstPortMonitor->Stop();
            break;
        default:;
    }
}

bool MainWindow::OnKeyPress(uint64_t keyval) {
    switch (keyval) {
        case 'q':
            DeleteMe();
            return true;
        case 's':
            m_pLstPortMonitor->Start();
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
