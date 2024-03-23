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

class MqttClient
{
public:
    MqttClient();
    ~MqttClient();
    int Connect();
    bool Subscribe(std::string topic);
    bool Publish(std::string topic, std::string payload);
    bool IsConnect();

private:
    static int32_t StateLogcb(int32_t code, char *message);
    static void MqttEventHandler(void *handle, const aiot_mqtt_event_t *event, void *userdata);
    static void DmRecvHandler(void *dm_handle, const aiot_dm_recv_t *recv, void *userdata);

private:
    std::string product_key_;
    std::string device_name_;
    std::string device_secret_;
    std::string mqtt_host_;
    uint16_t port_;
    std::mutex lock_;
    void *dm_handle_ = nullptr;
    void *mqtt_handle_ = nullptr;
    aiot_sysdep_portfile_t g_aiot_sysdep_portfile_;
    const char *ali_ca_cert_;
    aiot_sysdep_network_cred_t cred_;
    static constexpr const char *TAG = "mqtt_client";
};

#endif // MASTER_MQTT_CLIENT_H_
