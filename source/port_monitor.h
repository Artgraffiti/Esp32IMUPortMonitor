#ifndef PORT_MONITOR_H
#define PORT_MONITOR_H

#include <stdatomic.h>
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
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

class PortMonitor : public List {
   public:
    PortMonitor() { isRunning = false; }
    ~PortMonitor() { Stop(); }
    void Start();
    void Stop();

   private:
    int port_fd;
    struct termios newPortSettings, oldPortSettings;
    std::atomic_bool isRunning;
    std::thread thread_read;

    void Setup();
    void Run();
};

#endif