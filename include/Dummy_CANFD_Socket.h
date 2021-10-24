/*******************************************************************************
 \project   INFM_HIL_Interface
 \file      Dummy_CANFD_Socket.h
 \brief     Provides functions for the configuration of an CAN/CANFD interface
            and the setup of an CAN/CANFD socket.
 \author    Matthias Bank
 \version   1.0.0
 \date      24.10.2021
 ******************************************************************************/
#ifndef DUMMY_CAN_DUT_DUMMY_CANFD_SOCKET_H
#define DUMMY_CAN_DUT_DUMMY_CANFD_SOCKET_H


/*******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include <linux/can.h>


/*******************************************************************************
 * DEFINES
 ******************************************************************************/
#define ERR_IF_NOT_FOUND            -1
#define ERR_SOCKET_FAILED           -2
#define ERR_SETUP_FAILED            -3
#define ERR_MSG_SIZE                -4
#define ERR_MSG_GENERATION          -5
#define ERR_LOG_WRITE               -6
#define ERR_CLOCK_GETTIME           -7
#define ERR_SEND_FAILED             -8
#define ERR_RECV_FAILED             -9
#define ERR_SETSOCKOPT_FAILED       -10
#define ERR_CANSTOP_FAILED          -11
#define ERR_SETBITRATE_FAILED       -12
#define ERR_CANSTART_FAILED         -13
#define ERR_MALLOC_FAILED           -14
#define ERR_REALLOC_FAILED          -15
#define ERR_IFDOWN_FAILED           -16
#define ERR_IFUP_FAILED             -17
#define ERR_IPLINKSET_FAILED        -18
#define RET_E_OK                     0


/*******************************************************************************
 * FUNCTION DECLARATIONS
 ******************************************************************************/

/**
 * Prepares the bitrate of the CAN/CANFD interface
 */
extern int prepareCanInterface();


/**
 * Creates and binds a CAN/CANFD socket on an interface.
 * Support for CANFD frames can be enabled if needed.
 *
 * @param socketFD  - Storage for the created socket descriptor
 * @param addr     -  Storage for the sockaddr_can of the socket
 */
extern int setupSocket(int *const socketFD, struct sockaddr_can *const addr);


#endif //DUMMY_CAN_DUT_DUMMY_CANFD_SOCKET_H


/*******************************************************************************
 * END OF FILE
 ******************************************************************************/
