#include "port_monitor.h"

void PortMonitor::Start() {
    if (!isRunning) {
        isRunning = true;
        Setup();
        thread_read = std::thread(&PortMonitor::Run, this);
    }
}

void PortMonitor::Stop() {
    if (isRunning) {
        isRunning = false;
        if (thread_read.joinable()) {
            thread_read.join();
        }
    }
}

void PortMonitor::Setup() {
    port_fd = open("/dev/ttyACM0", O_RDONLY | O_NOCTTY);
    if (port_fd < 0) {
        perror("Error opening port");
        exit(EXIT_FAILURE);
    }

    tcgetattr(port_fd, &oldPortSettings);
    newPortSettings = oldPortSettings;

    cfsetispeed(&newPortSettings, B115200);

    newPortSettings.c_cflag &= ~PARENB;
    newPortSettings.c_cflag &= ~CSTOPB;
    newPortSettings.c_cflag &= ~CSIZE;
    newPortSettings.c_cflag |= CS8;

    newPortSettings.c_cflag &= ~CRTSCTS;
    newPortSettings.c_cflag |= CREAD | CLOCAL;

    newPortSettings.c_iflag &= ~(INLCR | ICRNL | IXON | IXOFF | IXANY);

    newPortSettings.c_lflag &=
        ~(ICANON | ECHO | ECHOE | ISIG | ECHOCTL | ECHOKE);

    newPortSettings.c_oflag &= ~OPOST;
    newPortSettings.c_oflag &= ~ONLCR;

    newPortSettings.c_cc[VMIN] = 0;
    newPortSettings.c_cc[VTIME] = 2;

    if ((tcsetattr(port_fd, TCSANOW, &newPortSettings)) != 0) {
        fprintf(stderr, "ERROR! in Setting new termios attributes\n");
        exit(EXIT_FAILURE);
    }
}

void PortMonitor::Run() {
    size_t buff_sz = 1024, line_sz = 512;
    char read_buffer[buff_sz];
    char line[line_sz];
    int bytes_read = 0;
    int line_pos = 0;

    int stop_signal = 0;
    while (isRunning) {
        bytes_read = read(port_fd, &read_buffer, sizeof(read_buffer));

        if (bytes_read == 0) {
            fprintf(stderr, "Timeout reached, no data received\n");
            isRunning = false;
            break;
        } else if (bytes_read < 0) {
            perror("Read Error");
            isRunning = false;
            break;
        }

        // Пользовательское построчное чтение
        for (int i = 0; i < bytes_read; i++) {
            if ((0 < read_buffer[i] && read_buffer[i] < 127) && // Only ASCII
                (read_buffer[i] == '\n' || read_buffer[i] == '\0' ||
                line_pos >= (int)(sizeof(line) - 1))) {
                line[line_pos] = '\0';  // Завершаем строку
                Text *pTxt = new Text(line);
                pTxt->SetFont(0, 16, -1, -1);
                pTxt->SetAlignment(TEXT_ALIGNV_MASK);
                Insert(0, pTxt);
                line_pos = 0;  // Сбрасываем позицию для следующей строки
            } else {
                line[line_pos++] = read_buffer[i];  // Добавляем символ в строку
            }
        }
    }

    if ((tcsetattr(port_fd, TCSANOW, &oldPortSettings)) != 0)
        fprintf(stderr, "ERROR! in restore old termios attributes\n");
    close(port_fd);
}

void PortMonitor::Insert(const uint16_t position, Window *pElement, void *value) {
    std::lock_guard<std::mutex> lock(mtx);
    List::Insert(position, pElement, value);
}

void PortMonitor::Clear() {
    std::lock_guard<std::mutex> lock(mtx);
    List::Clear();
}
