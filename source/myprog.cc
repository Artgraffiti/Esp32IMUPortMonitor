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
    EVENT_BTN_PORT_MONITOR = 5,
    EVENT_BTN_VISUALIZATION,
    EVENT_BTN_CLEAR,
    EVENT_BTN_START,
    EVENT_BTN_STOP,
};

class MainWindow : public Window {
   public:
    MainWindow() { m_ClassName = __FUNCTION__; }

    void OnCreate();
    void OnDraw(Context *cr);
    bool OnKeyPress(uint64_t value);
    void OnNotify(Window *child, uint32_t type, const Point &position);

   private:
    TextButton *m_pBtnPortMonitor, *m_pBtnVisualization, *m_pBtnClear, *m_pBtnStart, *m_pBtnStop;
    Scroll *m_pSclPortMonitor;
    PortMonitor *m_pPortMonitor;
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
    m_pSclPortMonitor->SetBackColor(WIN_BCK_COLOR);
    AddChild(m_pSclPortMonitor, Point(0, btns_height), Rect(WIN_WIDTH, WIN_HEIGHT - 2 * btns_height));
    m_pPortMonitor = new PortMonitor("/dev/ttyACM0");
    m_pPortMonitor->SetElementHeight(PM_LINE_HEIGHT);
    m_pPortMonitor->SetSelBackColor(RGB(0, 1, 0));
    m_pSclPortMonitor->SetDataWindow(m_pPortMonitor);

#if 0
    for (int i = 0; i < 1000; i++) {
        char buff[20];
        sprintf(buff, "Hello %d", i);
        Text *pTxt = new Text(buff);
        pTxt->SetFont(0, 16, -1, -1);
        pTxt->SetAlignment(TEXT_ALIGNV_MASK);
        m_pPortMonitor->Insert(0, pTxt);
    }
#endif

    btns_width = WIN_WIDTH / 3;
    m_pBtnClear = new TextButton("Очистить", EVENT_BTN_CLEAR);
    AddChild(m_pBtnClear, Point(0, WIN_HEIGHT - btns_height), Rect(btns_width, btns_height));
    m_pBtnClear->SetBackColor(BTNS_COLOR);
    m_pBtnClear->SetDarkColor(BTNS_COLOR);
    m_pBtnClear->SetLiteColor(BTNS_COLOR);
    m_pBtnClear->SetFrameColor(BTNS_FRAME_COLOR);
    m_pBtnClear->SetFontSize(26);
    m_pBtnClear->SetTextColor(H_RGB(0xffffff));

    m_pBtnStart = new TextButton("Старт", EVENT_BTN_START);
    AddChild(m_pBtnStart, Point(btns_width, WIN_HEIGHT - btns_height), Rect(btns_width, btns_height));
    m_pBtnStart->SetBackColor(BTNS_COLOR);
    m_pBtnStart->SetDarkColor(BTNS_COLOR);
    m_pBtnStart->SetLiteColor(BTNS_COLOR);
    m_pBtnStart->SetFrameColor(BTNS_FRAME_COLOR);
    m_pBtnStart->SetFontSize(26);
    m_pBtnStart->SetTextColor(H_RGB(0xffffff));

    m_pBtnStop = new TextButton("Стоп", EVENT_BTN_STOP);
    AddChild(m_pBtnStop, Point(2 * btns_width, WIN_HEIGHT - btns_height), Rect(btns_width, btns_height));
    m_pBtnStop->SetBackColor(BTNS_COLOR);
    m_pBtnStop->SetDarkColor(BTNS_COLOR);
    m_pBtnStop->SetLiteColor(BTNS_COLOR);
    m_pBtnStop->SetFrameColor(BTNS_FRAME_COLOR);
    m_pBtnStop->SetFontSize(26);
    m_pBtnStop->SetTextColor(H_RGB(0xffffff));

    CaptureKeyboard(this);
}

void MainWindow::OnDraw(Context *cr) {
    std::cout << "MainWindow::OnDraw()" << std::endl;
    Rect wsize = GetInteriorSize();

    cr->SetColor(WIN_BCK_COLOR);
    cr->FillRectangle(Point(0, 0), wsize);
    CaptureKeyboard(this);
}

void MainWindow::OnNotify(Window *child, uint32_t type, const Point &position) {
    std::cout << "MainWindow::OnNotify()" << std::endl;

    switch (type) {
        case EVENT_BTN_PORT_MONITOR:
            m_pSclPortMonitor->Show();
            ReDraw();
            break;
        case EVENT_BTN_VISUALIZATION:
            m_pPortMonitor->Stop();
            m_pSclPortMonitor->Hide();
            ReDraw();
            break;
        case EVENT_BTN_CLEAR:
            m_pPortMonitor->Clear();
            break;
        case EVENT_BTN_START:
            m_pPortMonitor->Clear();
            m_pPortMonitor->Start();
            break;
        case EVENT_BTN_STOP:
            m_pPortMonitor->Stop();
            break;
        default:;
    }
}

bool MainWindow::OnKeyPress(uint64_t keyval) {
    switch (keyval) {
        case 'q':
            DeleteMe();
            return true;
        case 'c':
            m_pSclPortMonitor->Show();
            m_pPortMonitor->Start();
            break;
        case 's':
            m_pPortMonitor->Stop();
            break;
        case KEY_Esc:
            m_pPortMonitor->Clear();
            break;
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
