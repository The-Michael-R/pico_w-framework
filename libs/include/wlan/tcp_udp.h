/** ****************************************************************************
 * @file   tcp_udp.h
 *
 * @author Michael R.
 *
 * @brief  Receives and send TCP and UDP packets via WLAN
 *
 * @date   2023-09-18
 **************************************************************************** */

#ifndef TCP_UDP_H
#define TCP_UDP_H

/* --- Includes ------------------------------------------------------------- */

#include <stdint.h>

#include "global/error_types.h"


/* --- Public macro definitions --------------------------------------------- */

#define MAX_UDP_BUFFER (128U)

#ifndef HOST_LOG_PORT
    #define HOST_LOG_PORT (54323U)
#endif

/* --- Public type/struct definitions --------------------------------------- */

typedef enum eTcpUdpSocketType_tag
{
    IP_TCP,
    IP_UDP,
    IP_NUMEL
} eTcpUdpSocketType_t;

/* --- Public variables ----------------------------------------------------- */

/* --- Public function prototypes ------------------------------------------- */

eRetVal_t eTcpUdpRtosInit(void);
eRetVal_t eTcpUdpOpenSocket(const eTcpUdpSocketType_t eType, const uint16_t uPort);
void vTcpUdpCloseSocket(const eTcpUdpSocketType_t eType);

void vTcpUdpSendTcp(char *const cpMessage);

void vTcpUdpPrintUdp(char *const cpMessage);

#endif /* TCP_UDP_H */
