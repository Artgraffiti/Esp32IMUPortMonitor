#include "port_monitor.h"

#include <cstdio>
#include <regex>
#include <string>

void remove_color_sequences(std::string &str) {
    // Регулярное выражение для ANSI escape sequences
    std::regex color_regex("\033\\[[0-9;]*m");
    str = std::regex_replace(str, color_regex, "");
}

void PortMonitor::OnDraw(Context *cr) {
    cr->SetColor(GetBackColor());
    cr->FillRectangle(Point(0, 0), GetInteriorSize());
}

void PortMonitor::SetPortPath(const std::string &port_path) {
    portPath = port_path;
}

int PortMonitor::Start() {
    int err = 0;
    if (!isRunning) {
        isRunning = true;
        err = Setup();
        if (err != 0) {
            isRunning = false;
            return err;
        } 
        thread_read = std::thread(&PortMonitor::Run, this);
    }
    return 0;
}

void PortMonitor::Stop() {
    if (isRunning) {
        isRunning = false;
        if (thread_read.joinable()) {
            thread_read.join();
        }
    }
}

int PortMonitor::Setup() {
    port_fd = open(portPath.c_str(), O_RDONLY | O_NOCTTY);
    if (port_fd < 0) {
        perror("Error opening port");
        return -1;
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
        return -1;
    }
    return 0;
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
            continue;
        } else if (bytes_read < 0) {
            perror("Read Error");
            isRunning = false;
            break;
        }

        // Пользовательское построчное чтение
        for (int i = 0; i < bytes_read; i++) {
            if ((0 < read_buffer[i] && read_buffer[i] < 127) &&  // Only ASCII
                (read_buffer[i] == '\n' || read_buffer[i] == '\0' ||
                 line_pos >= (int)(sizeof(line) - 1))) {
                line[line_pos] = '\0';  // Завершаем строку
                line_pos = 0;  // Сбрасываем позицию для следующей строки
                std::string formated_line = line;
                remove_color_sequences(formated_line);
                struct rotation data;
                sscanf(formated_line.c_str(), "%*[^r]roll=%f%*[^p]pitch=%f",
                       &data.roll, &data.pitch);

                // Add new line as element
                Text *pTxt = new Text(formated_line.c_str());
                pTxt->SetFont(0, 16, -1, -1);
                pTxt->SetAlignment(TEXT_ALIGNV_MASK);
                Insert(0, pTxt, &data);
            } else {
                line[line_pos++] = read_buffer[i];  // Добавляем символ в строку
            }
        }
    }

    if ((tcsetattr(port_fd, TCSANOW, &oldPortSettings)) != 0)
        fprintf(stderr, "ERROR! in restore old termios attributes\n");
    close(port_fd);
}

void PortMonitor::Insert(const uint16_t position, Window *pElement,
                         void *value) {
    std::lock_guard<std::mutex> lock(mtx);
    List::Insert(position, pElement, value);
}

void PortMonitor::Clear() {
    std::lock_guard<std::mutex> lock(mtx);
    List::Clear();
}
