/* *****************************************************************************
 * File:   app_socket_stdio.c
 * Author: Dimitar Lilov
 *
 * Created on 2022 06 18
 * 
 * Description: ...
 * 
 **************************************************************************** */

/* *****************************************************************************
 * Header Includes
 **************************************************************************** */
#include "app_socket_stdio.h"


#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "drv_socket.h"

#include "esp_event.h"
#include "esp_log.h"

/* *****************************************************************************
 * Configuration Definitions
 **************************************************************************** */
#define TAG "app_socket_stdio"

#define APP_SOCKET_STDIO_SEND_BUFFER_SIZE  (32 * 1024)
#define APP_SOCKET_STDIO_RECV_BUFFER_SIZE  1024

/* *****************************************************************************
 * Constants and Macros Definitions
 **************************************************************************** */

/* *****************************************************************************
 * Enumeration Definitions
 **************************************************************************** */

/* *****************************************************************************
 * Type Definitions
 **************************************************************************** */

/* *****************************************************************************
 * Function-Like Macros
 **************************************************************************** */

/* *****************************************************************************
 * Prototype of functions definitions for Variables
 **************************************************************************** */

/* *****************************************************************************
 * Variables Definitions
 **************************************************************************** */

StreamBufferHandle_t stdio_stream_buffer_send = NULL;
StreamBufferHandle_t stdio_stream_buffer_recv = NULL;

drv_socket_t socket_stdio = 
{
    .cName = "stdio",
    .bServerType = false,
    .nSocketConnectionsCount = 0,
    .nSocketIndexPrimer = {-1},
    .nSocketIndexServer = -1,
    .cURL = DRV_SOCKET_DEFAULT_URL,
    .cHostIP = DRV_SOCKET_DEFAULT_IP,
    //.cHostIP = "84.40.115.3",
    //.cHostIP = "192.168.3.2",
    //.cHostIP = "192.168.0.1",
    .u16Port = 3333,
    //.u16Port = 3336,
    .bActiveTask = false,
    .bConnected = false,
    .bSendEnable = false,
    .bSendFillEnable = true,
    .bAutoSendEnable = false,
    .bIndentifyForced = true,
    .bIndentifyNeeded = false,
    .bResetSendStreamOnConnect = false,
    //.bPingUse = true,
    .bPingUse = false,
    .bLineEndingFixCRLFToCR = true,
    .bPermitBroadcast = false,
    .bConnectDeny = false,

    .bPreventOverflowReceivedData = false,
    .pTask = NULL,
    .adapter_interface = {DRV_SOCKET_IF_DEFAULT,  DRV_SOCKET_IF_BACKUP },
    .onConnect = NULL,
    .onReceive = NULL,
    .onSend = NULL,
    .onDisconnect = NULL,
    .onReceiveFrom = NULL,
    .onSendTo = NULL,
    .address_family = AF_INET,
    .protocol = IPPROTO_IP,
    .protocol_type = SOCK_STREAM,
    .pSendStreamBuffer = {&stdio_stream_buffer_send},
    .pRecvStreamBuffer = {&stdio_stream_buffer_recv},
};

/* *****************************************************************************
 * Prototype of functions definitions
 **************************************************************************** */

/* *****************************************************************************
 * Functions
 **************************************************************************** */

void app_socket_stdio_init(void)
{
    drv_stream_init(socket_stdio.pSendStreamBuffer[0], APP_SOCKET_STDIO_SEND_BUFFER_SIZE, "stdio_sock_send");
    drv_stream_init(socket_stdio.pRecvStreamBuffer[0], APP_SOCKET_STDIO_RECV_BUFFER_SIZE, "stdio_sock_recv");
}

void app_socket_stdio_task(void)
{
    drv_socket_task(&socket_stdio, -1);
}

int app_socket_stdio_send(const char* pData, int size)
{
    if ((socket_stdio.bSendEnable) || (socket_stdio.bSendFillEnable))
    {
        //ESP_LOGI(TAG, "app_socket_stdio_send %d bytes", size);    esp log here not permitted
        //ESP_LOG_BUFFER_CHAR(TAG, pData, size);
        
        return drv_stream_push(socket_stdio.pSendStreamBuffer[0], (uint8_t*)pData, size);
    }
    else
    {
        return 0;
    }
}

int app_socket_stdio_recv(char* pData, int size)
{
    if (socket_stdio.bConnected)
    {
        //ESP_LOGI(TAG, "app_socket_stdio_recv %d bytes", size);
        //ESP_LOG_BUFFER_CHAR(TAG, pData, size);
        return drv_stream_pull(socket_stdio.pRecvStreamBuffer[0], (uint8_t*)pData, size);
        //return 0; 
    }
    else
    {
        return 0;
    }
}