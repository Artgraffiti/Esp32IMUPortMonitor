#include "window.h"
#include "GUI.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 500

class MainWindow : public Window {
   public:
    MainWindow() {}
    ~MainWindow() {}

    void OnCreate();
    bool OnKeyPress(uint64_t value);
    void OnSizeChanged();

   public:
    const char *m_filename;

   private:
};

void MainWindow::OnCreate() { CaptureKeyboard(this); }

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
