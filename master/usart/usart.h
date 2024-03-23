#ifndef MASTER_USART_H_
extern "C"
{
#include <wiringSerial.h>
}
#include <atomic>
#include <thread>
#define USART_DEVICE "/dev/ttyAMA0"
#define USART_BAUD 9600

class Usart
{
public:
    Usart();
    ~Usart();
    // open usart device
    int Open();
    void Run();
    // write data to usart
    int Write(const char *buf);
    // Read data from usart device
    int Read(char *buf, int len);

private:
    int fd_;
    std::unique_ptr<std::thread> thread_{nullptr};
    std::atomic_bool stopped_{false};
    static constexpr const char *TAG = "usart";
};

#endif // !MASTER_USART_H_