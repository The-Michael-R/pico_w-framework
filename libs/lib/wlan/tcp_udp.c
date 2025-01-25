/** ****************************************************************************
 * @file   tcp_udp.c
 *
 * @author Michael R.
 *
 * @brief  Receives and send TCP and UDP packets via WLAN
 *
 * @date   2023-09-18
 **************************************************************************** */

/* --- Includes ------------------------------------------------------------- */

// libc includes
#include <string.h>

// pico-sdk includes
#include "pico/cyw43_arch.h"
#include "lwip/sockets.h"
#include "lwip/udp.h"

// FreeRTOS includes
#include "FreeRTOS.h" /* Must come first. */
#include "task.h"
#include "queue.h"

// Project includes
#include "wlan/tcp_udp.h"
#include "wlan/wlan_state.h"

#include "global/debug_print.h"

/* --- Local macro definitions ---------------------------------------------- */

#define HOST_IP_ADDR ((const char*)IP_ADDR_ANY) // Broadcast
#define TCP_UDP_SEN_QUEUE_LEN (5U)


/* --- Local type/struct definitions ---------------------------------------- */

typedef struct sUdpConf_tag
{
    ip_addr_t tIp;
    uint16_t uPort;

    QueueHandle_t xUdpSendPointerQueue;
} sUdpConf_t;


typedef struct sTcpConf_tag
{
    int iSocket;
} sTcpConf_t;

typedef struct sTcpUdpState_tag
{
    sUdpConf_t sUdp;
    sTcpConf_t sTcp;
} sTcpUdpState_t;


/* --- Static variables ----------------------------------------------------- */

static sTcpUdpState_t sTcpUdpState;
struct pbuf *sPb;


/* --- Static function prototypes ------------------------------------------- */

/* --- Public functions ----------------------------------------------------- */

void vTcpUdpInit(void)
{
    DBG_PR(DBG_INFO, FN_TCPUDP, "\n");
    sTcpUdpState.sTcp.iSocket = -1;

    sTcpUdpState.sUdp.tIp.addr = ipaddr_addr(HOST_IP_ADDR);
    sTcpUdpState.sUdp.xUdpSendPointerQueue = xQueueCreate(TCP_UDP_SEN_QUEUE_LEN, sizeof(char *));
}

eRetVal_t eTcpUdpOpenSocket(const eTcpUdpSocketType_t eType, const uint16_t uPort)
{
    DBG_PR(DBG_INFO, FN_TCPUDP, "\n");

    eRetVal_t eRetVal = ErrNoError;

    if (eType == IP_UDP)
    {
        sTcpUdpState.sUdp.uPort = uPort;
    }
    else if (eType == IP_TCP)
    {

    }

    return (eRetVal);
}


void vTcpUdpCloseSocket(const eTcpUdpSocketType_t eType)
{
    if (eType == IP_UDP)
    {
    }
    else if (eType == IP_TCP)
    {
        lwip_shutdown(sTcpUdpState.sTcp.iSocket, 0);
        sTcpUdpState.sTcp.iSocket = -1;
    }
}


void vTcpUdpSendTcp(char *const cpMessage)
{
    if (sTcpUdpState.sTcp.iSocket != -1)
    {
        lwip_send(
            sTcpUdpState.sTcp.iSocket,
            cpMessage,
            strlen(cpMessage),
            0);
    }
}


void vTcpUdpPrintUdp(char *const cpMessage)
{
    struct udp_pcb* sPcb;

    //Make sure that the buffer is NULL terminated
    cpMessage[MAX_UDP_BUFFER - 1U] = '\0';

    sPb = pbuf_alloc(PBUF_TRANSPORT, MAX_UDP_BUFFER, PBUF_REF);
    sPb->payload = cpMessage;
    sPb->len = strlen(cpMessage);
    sPb->tot_len = strlen(cpMessage);

    sPcb = udp_new();

    cyw43_arch_lwip_begin();
    udp_sendto(sPcb, sPb, &sTcpUdpState.sUdp.tIp, sTcpUdpState.sUdp.uPort);
    cyw43_arch_lwip_end();
    udp_remove(sPcb);

    pbuf_free(sPb);
}

/* --- Static functions ----------------------------------------------------- */
