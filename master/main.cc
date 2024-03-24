#include <iostream>
#include <string>
#include "xfframework/config.h"
#include "xfframework/logging.h"
#include "usart/usart.h"
#include "mqtt/mqtt_client.h"
int main()
{
    std::cout << "hospital 5g project!" << std::endl;
    // XfConfig config;
    // config.Open(TEST_CONFIG_FILE);
    // Usart usart;
    Usart usart;
    std::function<int(const char *buf)> usart_send_cb =
        std::bind(&Usart::Write, &usart, std::placeholders::_1);

    MqttClient mqtt_client;
    mqtt_client.AddCallBack(usart_send_cb);

    std::function<bool(std::string)> mqtt_send_cb = std::bind(&MqttClient::Publish, &mqtt_client, std::placeholders::_1);
    usart.AddCallBack(mqtt_send_cb);

    while (1)
    {
        /* code */
    }

    return 0;
}