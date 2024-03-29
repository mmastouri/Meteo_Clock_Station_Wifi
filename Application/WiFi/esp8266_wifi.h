/**************************************************************************//**
 * @file     esp8266_wifi.h
 * @version  V1.10
 * @brief    M480 series ESP8266 WiFi driver header file
 *
 * @copyright (C) 2019 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#ifndef __ESP_WIFI_H
#define __ESP_WIFI_H

#ifdef __cplusplus
 extern "C" {
#endif  

#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
   
#define ESP_PRINTF( X )

#define ESP_WIFI_DATA_SIZE           3072
#define ESP_WIFI_SEND_SIZE           1200
#define ESP_WIFI_IPD_SIZE            ESP_WIFI_DATA_SIZE

#define ESP_WIFI_IPD_HIGH_LEVEL     (ESP_WIFI_IPD_SIZE - 1024)

#define ESP_WIFI_NONBLOCK_SEND_TO   2000
#define ESP_WIFI_NONBLOCK_RECV_TO   200
#define ESP_WIFI_SHORT_RECV_TO      10

#define AT_OK_STRING                "OK\r\n"
#define AT_FAIL_STRING              "FAIL\r\n"
#define AT_ERROR_STRING             "ERROR\r\n"
#define AT_SEND_STRING              ">"
#define AT_RECV_STRING              "+IPD,"
#define AT_CLOSE_STRING             ",CLOSED"

/* List of commands */
#define CMD_NONE                    0x00
#define CMD_SEND                    0x01
#define CMD_RECV                    0x02


typedef enum {
    ESP_WIFI_STATUS_OK          = 0,
    ESP_WIFI_STATUS_ERROR       = 1,
    ESP_WIFI_STATUS_TIMEOUT     = 2,
    ESP_WIFI_STATUS_SEND        = 3,
    ESP_WIFI_STATUS_RECV        = 4,
    ESP_WIFI_STATUS_CLOSE       = 5
} ESP_WIFI_Status_t;

typedef enum {
    ESP_WIFI_TCP    = 0,
    ESP_WIFI_UDP    = 1,
    ESP_WIFI_SSL    = 2
}ESP_WIFI_ConnType_t;

typedef struct {
    uint8_t LinkID;
    ESP_WIFI_ConnType_t Type;
    uint8_t RemoteIP[4];
    uint16_t RemotePort;
    uint16_t TcpKeepAlive;
    uint16_t UdpLocalPort;
    uint8_t UdpMode;
    uint8_t Status;
    BaseType_t IsConnected;
} ESP_WIFI_Conn_t;

typedef struct {
    uint8_t CmdData[ESP_WIFI_DATA_SIZE];
    uint8_t ActiveCmd;
    uint32_t Timeout;
    TickType_t AvailableTick;
    BaseType_t IsConnected;
    BaseType_t IsMultiConn;

    uint8_t StaIpAddr[4];
    uint8_t StaMacAddr[6];
    uint8_t ApIpAddr[4];
    uint8_t ApMacAddr[6];
} ESP_WIFI_Object_t;

typedef struct {
	uint8_t LinkID;
    uint16_t DataLength;
    uint8_t Data[ESP_WIFI_IPD_SIZE + 1];
} ESP_WIFI_IPD_t;

#define ESP_MAX_SSID_LEN   64
#define ESP_MAX_BSSID_LEN  64
#define ESP_MAX_SOCKETS  12
typedef enum
{
    eWiFiSecurityOpen = 0,    /**< Open - No Security. */
    eWiFiSecurityWEP,         /**< WEP Security. */
    eWiFiSecurityWPA,         /**< WPA Security. */
    eWiFiSecurityWPA2,        /**< WPA2 Security. */
    eWiFiSecurityWPA2_ent,    /**< WPA2 Enterprise Security. */
    eWiFiSecurityWPA3,        /**< WPA3 Security. */
    eWiFiSecurityNotSupported /**< Unknown Security. */
} ESP_Security_t;

typedef struct
{
    char cSSID           [ ESP_MAX_SSID_LEN + 1 ];   
    uint8_t ucBSSID      [ ESP_MAX_BSSID_LEN ]; 
    ESP_Security_t     xSecurity;                   
    int8_t cRSSI;                               
    int8_t cChannel;                            
    uint8_t ucHidden;                           
} ESP_Scan_t;

typedef struct
{
    char    SSID           [ ESP_MAX_SSID_LEN + 1 ];   
    char    PWD            [ ESP_MAX_SSID_LEN ];                        
} ESP_AvHotspot_t;

/* Exported functions */
ESP_WIFI_Status_t ESP_WIFI_Init( ESP_WIFI_Object_t * pxObj );
ESP_WIFI_Status_t ESP_WIFI_Connect( ESP_WIFI_Object_t * pxObj, const char * SSID, const char * Password );
ESP_WIFI_Status_t ESP_WIFI_Disconnect( ESP_WIFI_Object_t * pxObj );
ESP_WIFI_Status_t ESP_WIFI_Reset( ESP_WIFI_Object_t * pxObj );
ESP_WIFI_Status_t ESP_WIFI_Scan( ESP_WIFI_Object_t * pxObj, ESP_Scan_t * pxBuffer, uint8_t ucNumNetworks );
ESP_WIFI_Status_t ESP_WIFI_GetNetStatus( ESP_WIFI_Object_t * pxObj );
void ESP_WIFI_Reset_Ipd( ESP_WIFI_Conn_t * pxConn );
ESP_WIFI_Status_t ESP_WIFI_GetConnStatus( ESP_WIFI_Object_t * pxObj, ESP_WIFI_Conn_t * pxConn );
ESP_WIFI_Status_t ESP_WIFI_GetHostIP( ESP_WIFI_Object_t * pxObj, char * pcHost, uint8_t * pucIPAddr );
ESP_WIFI_Status_t ESP_WIFI_SetMultiConn( ESP_WIFI_Object_t * pxObj, uint8_t mode );
ESP_WIFI_Status_t ESP_WIFI_StartClient( ESP_WIFI_Object_t * pxObj, ESP_WIFI_Conn_t *xConn );
ESP_WIFI_Status_t ESP_WIFI_StopClient( ESP_WIFI_Object_t * pxObj, ESP_WIFI_Conn_t *xConn );
ESP_WIFI_Status_t ESP_WIFI_Send( ESP_WIFI_Object_t * pxObj, ESP_WIFI_Conn_t *xConn, uint8_t *pcBuf, 
                                 uint16_t usReqLen, uint16_t * usSendLen, uint32_t ulTimeout );
ESP_WIFI_Status_t ESP_WIFI_Recv( ESP_WIFI_Object_t * pxObj, ESP_WIFI_Conn_t * pxConn, uint8_t * pcBuf, 
                                 uint16_t usReqLen, uint16_t * usRecvLen, uint32_t ulTimeout );
BaseType_t ESP_WIFI_IsConnected( ESP_WIFI_Object_t * pxObj );
uint16_t ESP_WIFI_Get_Ipd_Size( ESP_WIFI_Conn_t * pxConn );


#ifdef __cplusplus
}
#endif
#endif /*__ESP_WIFI_H*/
