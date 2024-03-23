#include "mqtt_client.h"
#include "xfframework/config.h"
#include "xfframework/logging.h"
#include "version.h"
MqttClient::MqttClient()
{
    // load mqtt config
    XfConfig config;
    config.Open(CONFIG_FILE);

    config.Get("mqtt", "product_key", product_key_);
    XF_LOGT(INFO, TAG, "product_key %s\n", product_key_.c_str());
    config.Get("mqtt", "device_name", device_name_);
    XF_LOGT(INFO, TAG, "device_name %s\n", device_name_.c_str());
    config.Get("mqtt", "device_secret", device_secret_);
    XF_LOGT(INFO, TAG, "device_secret %s\n", device_secret_.c_str());
    config.Get("mqtt", "mqtt_host", mqtt_host_);
    XF_LOGT(INFO, TAG, "mqtt_host %s\n", mqtt_host_.c_str());
    config.Get("mqtt", "port", port_);
    XF_LOGT(INFO, TAG, "port %d\n", port_);
}
MqttClient::~MqttClient()
{
}

int32_t MqttClient::StateLogcb(int32_t code, char *message)
{
    printf("%s", message);
    XF_LOGT(INFO, TAG, "%s", message);
    return 0;
}

/* MQTT事件回调函数, 当网络连接/重连/断开时被触发, 事件定义见core/aiot_mqtt_api.h */
void MqttClient::MqttEventHandler(void *handle, const aiot_mqtt_event_t *event, void *userdata)
{
    switch (event->type)
    {
    /* SDK因为用户调用了aiot_mqtt_connect()接口, 与mqtt服务器建立连接已成功 */
    case AIOT_MQTTEVT_CONNECT:
    {
        printf("AIOT_MQTTEVT_CONNECT\n");
    }
    break;

    /* SDK因为网络状况被动断连后, 自动发起重连已成功 */
    case AIOT_MQTTEVT_RECONNECT:
    {
        printf("AIOT_MQTTEVT_RECONNECT\n");
    }
    break;

    /* SDK因为网络的状况而被动断开了连接, network是底层读写失败, heartbeat是没有按预期得到服务端心跳应答 */
    case AIOT_MQTTEVT_DISCONNECT:
    {
        char *cause = (event->data.disconnect == AIOT_MQTTDISCONNEVT_NETWORK_DISCONNECT) ? (char *)("network disconnect") : (char *)("heartbeat disconnect");
        printf("AIOT_MQTTEVT_DISCONNECT: %s\n", cause);
    }
    break;

    default:
    {
    }
    }
}

/* 用户数据接收处理回调函数 */
void MqttClient::DmRecvHandler(void *dm_handle, const aiot_dm_recv_t *recv, void *userdata)
{
    printf("demo_dm_recv_handler, type = %d\r\n", recv->type);

    switch (recv->type)
    {

    /* 属性上报, 事件上报, 获取期望属性值或者删除期望属性值的应答 */
    case AIOT_DMRECV_GENERIC_REPLY:
    {
        // demo_dm_recv_generic_reply(dm_handle, recv, userdata);
    }
    break;

    /* 属性设置 */
    case AIOT_DMRECV_PROPERTY_SET:
    {
        // demo_dm_recv_property_set(dm_handle, recv, userdata);
    }
    break;

    /* 异步服务调用 */
    case AIOT_DMRECV_ASYNC_SERVICE_INVOKE:
    {
        // demo_dm_recv_async_service_invoke(dm_handle, recv, userdata);
    }
    break;

    /* 同步服务调用 */
    case AIOT_DMRECV_SYNC_SERVICE_INVOKE:
    {
        // demo_dm_recv_sync_service_invoke(dm_handle, recv, userdata);
    }
    break;

    /* 下行二进制数据 */
    case AIOT_DMRECV_RAW_DATA:
    {
        // demo_dm_recv_raw_data(dm_handle, recv, userdata);
    }
    break;

    /* 二进制格式的同步服务调用, 比单纯的二进制数据消息多了个rrpc_id */
    case AIOT_DMRECV_RAW_SYNC_SERVICE_INVOKE:
    {
        // demo_dm_recv_raw_sync_service_invoke(dm_handle, recv, userdata);
    }
    break;

    /* 上行二进制数据后, 云端的回复报文 */
    case AIOT_DMRECV_RAW_DATA_REPLY:
    {
        // demo_dm_recv_raw_data_reply(dm_handle, recv, userdata);
    }
    break;

    default:
        break;
    }
}

int MqttClient::Connect()
{

    /* 安全凭据结构体, 如果要用TLS, 这个结构体中配置CA证书等参数 */
    /* 配置SDK的底层依赖 */
    aiot_sysdep_set_portfile(&g_aiot_sysdep_portfile_);
    /* 配置SDK的日志输出 */
    aiot_state_set_logcb(StateLogcb);
    /* 创建SDK的安全凭据, 用于建立TLS连接 */
    memset(&cred_, 0, sizeof(aiot_sysdep_network_cred_t));
    cred_.option = AIOT_SYSDEP_NETWORK_CRED_SVRCERT_CA; /* 使用RSA证书校验MQTT服务端 */
    cred_.max_tls_fragment = 16384;                     /* 最大的分片长度为16K, 其它可选值还有4K, 2K, 1K, 0.5K */
    cred_.sni_enabled = 1;                              /* TLS建连时, 支持Server Name Indicator */
    cred_.x509_server_cert = ali_ca_cert_;              /* 用来验证MQTT服务端的RSA根证书 */
    cred_.x509_server_cert_len = strlen(ali_ca_cert_);  /* 用来验证MQTT服务端的RSA根证书长度 */

    /* 创建1个MQTT客户端实例并内部初始化默认参数 */
    mqtt_handle_ = aiot_mqtt_init();
    if (mqtt_handle_ == NULL)
    {
        printf("aiot_mqtt_init failed\n");
        XF_LOGT(INFO, TAG, "aiot_mqtt_init failed");
        return -1;
    }

    /* 配置MQTT服务器地址 */
    aiot_mqtt_setopt(mqtt_handle_, AIOT_MQTTOPT_HOST, (void *)mqtt_host_.c_str());
    /* 配置MQTT服务器端口 */
    aiot_mqtt_setopt(mqtt_handle_, AIOT_MQTTOPT_PORT, (void *)&port_);
    /* 配置设备productKey */
    aiot_mqtt_setopt(mqtt_handle_, AIOT_MQTTOPT_PRODUCT_KEY, (void *)product_key_.c_str());
    /* 配置设备deviceName */
    aiot_mqtt_setopt(mqtt_handle_, AIOT_MQTTOPT_DEVICE_NAME, (void *)device_name_.c_str());
    /* 配置设备deviceSecret */
    aiot_mqtt_setopt(mqtt_handle_, AIOT_MQTTOPT_DEVICE_SECRET, (void *)device_secret_.c_str());
    /* 配置网络连接的安全凭据, 上面已经创建好了 */
    aiot_mqtt_setopt(mqtt_handle_, AIOT_MQTTOPT_NETWORK_CRED, (void *)&cred_);
    /* 配置MQTT事件回调函数 */
    aiot_mqtt_setopt(mqtt_handle_, AIOT_MQTTOPT_EVENT_HANDLER, (void *)MqttEventHandler);

    /* 创建DATA-MODEL实例 */
    dm_handle_ = aiot_dm_init();
    if (dm_handle_ == NULL)
    {
        printf("aiot_dm_init failed");
        return -1;
    }
    /* 配置MQTT实例句柄 */
    aiot_dm_setopt(dm_handle_, AIOT_DMOPT_MQTT_HANDLE, mqtt_handle_);
    /* 配置消息接收处理回调函数 */
    aiot_dm_setopt(dm_handle_, AIOT_DMOPT_RECV_HANDLER, (void *)DmRecvHandler);

    uint8_t post_reply = 1;
    /* 配置是云端否需要回复post_reply给设备. 如果为1, 表示需要云端回复, 否则表示不回复 */
    aiot_dm_setopt(dm_handle_, AIOT_DMOPT_POST_REPLY, (void *)&post_reply);

    /* 与服务器建立MQTT连接 */
    int32_t res = aiot_mqtt_connect(mqtt_handle_);
    if (res < STATE_SUCCESS)
    {
        /* 尝试建立连接失败, 销毁MQTT实例, 回收资源 */
        aiot_dm_deinit(&dm_handle_);
        aiot_mqtt_deinit(&dm_handle_);
        XF_LOGT(ERROR, TAG, "aiot_mqtt_connect failed: -0x%04X\n\r\n", -res);
        XF_LOGT(ERROR, TAG, "please check variables like mqtt_host, produt_key, device_name, device_secret in demo\r\n");

        return -1;
    }

    return 0;
}
bool MqttClient::Subscribe(std::string topic)
{
    return true;
}
bool MqttClient::Publish(std::string topic, std::string payload)
{
    return true;
}
bool MqttClient::IsConnect()
{
    return true;
}