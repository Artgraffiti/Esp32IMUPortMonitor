#ifndef PORT_MONITOR_H
#define PORT_MONITOR_H

#include "window.h"
#include "text.h"
#include "list.h"
#include "scroll.h"
#include "image.h"
#include "button.h"
#include "GUI.h"

#undef min
#undef max

#include <thread>
#include <atomic>
#include <mutex>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

class PortMonitor : public List {
   public:
    PortMonitor() { isRunning = false; }
    ~PortMonitor() { Stop(); }
    void OnDraw(Context *cr);

    void Start();
    void Stop();

    void Insert(const uint16_t position, Window *pElement, void *value=nullptr);
    void Clear();
   
   private:
    int port_fd;
    struct termios newPortSettings, oldPortSettings;
    std::atomic_bool isRunning;
    std::mutex mtx;
    std::thread thread_read;

    void Delete(const uint16_t n);
    void Setup();
    void Run();
};

#endif