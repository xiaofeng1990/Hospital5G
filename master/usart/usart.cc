#include "usart.h"
#include <unistd.h>
#include <cstdio>
#include <string>
#include <thread>
#include "xfframework/logging.h"
#include <functional>
using namespace std;

Usart::Usart()
{
    Open();
    thread_.reset(new std::thread(std::bind(&Usart::Run, this)));
}

Usart::~Usart()
{
    // close usart device
    if (fd_ > 0)
        serialClose(fd_);

    stopped_.store(true, std::memory_order_release);
    if (thread_ != nullptr && thread_->joinable())
    {
        thread_->join();
    }
    thread_.reset();
}

// open usart device
int Usart::Open()
{
    fd_ = serialOpen(USART_DEVICE, USART_BAUD);
    if (fd_ >= 0)
    {
        XF_LOGT(INFO, TAG, "open usart ok\n");
        return 0;
    }
    else
    {
        XF_LOGT(ERROR, TAG, "open usart error\n");
        return -1;
    }
}

void Usart::Run()
{

    while (!stopped_.load(std::memory_order_consume))
    {
        char buf[1024];
        Read(buf, 1024);
        XF_LOGT(INFO, TAG, "usart get data %s\n", buf);
        // 收到数据不为空，就调用mqtt发送函数
    }
}

// write data to usart
int Usart::Write(const char *buf)
{
    serialPuts(fd_, buf);
    return 0;
}

// Read data from usart device
int Usart::Read(char *buf, int len)
{
    int avail = 0;
    int number = 0;
    do
    {
        int ret = read(fd_, &buf[number], len);
        number += ret;
        if (number >= len)
            break;

    } while (serialDataAvail(fd_));
    serialFlush(fd_);
    return number;
}
