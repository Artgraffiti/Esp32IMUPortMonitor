#ifndef PORT_MONITOR_H
#define PORT_MONITOR_H

#ifndef COMMON_INC
#define COMMON_INC
#include "window.h"
#include "text.h"
#include "list.h"
#include "scroll.h"
#include "image.h"
#include "button.h"
#include "GUI.h"
#endif

#undef min
#undef max

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#include <atomic>
#include <mutex>
#include <string>
#include <thread>

struct rotation {
    float roll, pitch;
};

class PortMonitor : public List {
   public:
    PortMonitor() : portPath(), isRunning(false) {}
    ~PortMonitor() { Stop(); }
    void OnDraw(Context *cr);

    bool IsRunning() { return isRunning; }
    void SetPortPath(const std::string &port_path);
    int Start();
    void Stop();

    void Insert(const uint16_t position, Window *pElement,
                void *value = nullptr);
    void Clear();

   private:
    std::string portPath;
    int port_fd;
    struct termios newPortSettings, oldPortSettings;
    std::atomic_bool isRunning;
    std::mutex mtx;
    std::thread thread_read;

    void Delete(const uint16_t n);
    int Setup();
    void Run();
};

#endif