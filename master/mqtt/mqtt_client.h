#ifndef MASTER_MQTT_CLIENT_H_
#define MASTER_MQTT_CLIENT_H_

extern "C"
{
#include "aiot_state_api.h"
#include "aiot_sysdep_api.h"
#include "aiot_mqtt_api.h"
#include "aiot_dm_api.h"
}

#include <string>
#include <mutex>
#include <thread>
#include <functional>
#include <vector>
class MqttClient
{
public:
    typedef std::function<int(const char *buf)> callback_t;
    MqttClient();
    ~MqttClient();
    int Connect();
    bool Subscribe(std::string topic);
    // bool Publish(std::string topic, std::string payload);
    bool Publish(std::string payload);
    bool IsConnect();
    bool AddCallBack(callback_t cb);

private:
    static int32_t StateLogcb(int32_t code, char *message);
    static void MqttEventHandler(void *handle, const aiot_mqtt_event_t *event, void *userdata);
    static void DmRecvHandler(void *dm_handle, const aiot_dm_recv_t *recv, void *userdata);
    void MqttRecvThread(void *args);
    void MqttProcessThread(void *args);
    static void DmRecvAsyncServiceInvoke(void *dm_handle, const aiot_dm_recv_t *recv, void *userdata);

private:
    std::string product_key_;
    std::string device_name_;
    std::string device_secret_;
    std::string mqtt_host_;
    std::string subscribe_topic_;
    std::string publish_topic_;
    uint16_t port_;
    std::mutex lock_;
    void *dm_handle_ = nullptr;
    void *mqtt_handle_ = nullptr;
    aiot_sysdep_network_cred_t cred_;

    std::unique_ptr<std::thread> mqtt_process_thread_{nullptr};
    std::unique_ptr<std::thread> mqtt_recv_thread_{nullptr};

    uint8_t mqtt_process_thread_running_ = 0;
    uint8_t mqtt_recv_thread_running_ = 0;
    static MqttClient *mqtt_this_;
    callback_t usart_send_cb_;

    static constexpr const char *TAG = "mqtt_client";
};

#endif // MASTER_MQTT_CLIENT_H_
