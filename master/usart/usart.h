#ifndef MASTER_USART_H_
extern "C"
{
#include <wiringSerial.h>
}
#include <atomic>
#include <thread>
#include <functional>
#define USART_DEVICE "/dev/ttyAMA0"
#define USART_BAUD 9600

class Usart
{
public:
    typedef std::function<bool(std::string)> callback_t;
    Usart();
    ~Usart();
    // open usart device
    int Open();
    void Run();
    // write data to usart
    int Write(const char *buf);
    // Read data from usart device
    int Read(char *buf, int len);
    bool AddCallBack(callback_t cb)
    {
        mqtt_send_cb_ = cb;
        return true;
    }

private:
    int fd_;
    std::unique_ptr<std::thread> thread_{nullptr};
    std::atomic_bool stopped_{false};
    callback_t mqtt_send_cb_ = nullptr;
    static constexpr const char *TAG = "usart";
};

#endif // !MASTER_USART_H_