/*******************************************************************************
 \project   INFM_HIL_Interface
 \file      Dummy_CANFD_Socket.c
 \brief     Provides functions for the configuration of an CAN/CANFD interface
            and the setup of an CAN/CANFD socket.
 \author    Matthias Bank
 \version   1.0.0
 \date      24.10.2021
 ******************************************************************************/


/*******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include <errno.h>
#include "Dummy_CANFD_Config.h"
#include "Dummy_CANFD_Socket.h"
#include <linux/can/raw.h>
#include <linux/can.h>
#include <net/if.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>


/*******************************************************************************
 * FUNCTION DEFINITIONS
 ******************************************************************************/

int prepareCanInterface(){

    int foundIF = 0;
    char line[255];
    FILE *file = fopen("/run/network/ifstate", "r");

    // Read each line in the file to check if the interface name is present
    while(fgets(line, sizeof(line), file) != NULL){

        if(strstr(line, INTERFACE) != NULL) {
            foundIF = 1;
            break;
        }

    }

    int size;
    char *command = NULL;

    // Check if the interface is configured
    if(foundIF){

        // Build the command for setting the interface down
        size = snprintf(command, 0, "/sbin/ifdown %s", INTERFACE) + 1;
        command = malloc(size);

        // Error handling
        if(command == NULL){
            printf("Error could not allocate memory\n");
            return ERR_MALLOC_FAILED;
        }

        snprintf(command, size, "/sbin/ifdown %s", INTERFACE);

        // Bring the interface down for configuration
        if(system(command) == -1){
            printf("Error could not bring the interface %s down\n", INTERFACE);
            return ERR_IFDOWN_FAILED;
        }

    }

    // Build the command for the interface configuration
    // Note: This is a workaround because of the bad CAN interface driver
    if(CANFD){

        // Build the command for the CAN interface configuration
        size = snprintf(command, 0, "sudo ip link set %s up type can bitrate %d sample-point %.3f"
                                    " dbitrate %d dsample-point %.3f fd on", INTERFACE, BITRATE, SAMPLEPOINT, DBITRATE, DSAMPLEPOINT) + 1;

        command = realloc(command, size);

        // Error handling
        if(command == NULL){
            printf("Error could not reallocate memory\n");
            return ERR_REALLOC_FAILED;
        }

        size = snprintf(command, size, "sudo ip link set %s up type can bitrate %d sample-point %.3f"
                                       " dbitrate %d dsample-point %.3f fd on", INTERFACE, BITRATE, SAMPLEPOINT, DBITRATE, DSAMPLEPOINT);

    }else{

        // Build the command for the CANFD interface configuration
        size = snprintf(command, 0, "sudo ip link set %s up type can bitrate %d sample-point %.3f"
                                    " dbitrate %d dsample-point %.3f fd off", INTERFACE, BITRATE, SAMPLEPOINT, DBITRATE, DSAMPLEPOINT) + 1;

        command = realloc(command, size);

        // Error handling
        if(command == NULL){
            printf("Error could not reallocate memory\n");
            return ERR_MALLOC_FAILED;
        }

        size = snprintf(command, size, "sudo ip link set %s up type can bitrate %d sample-point %.3f"
                                       " dbitrate %d dsample-point %.3f fd off", INTERFACE, BITRATE, SAMPLEPOINT, DBITRATE, DSAMPLEPOINT);

    }

    // Configure the interface
    if(system(command) == -1){
        printf("Error could not configure the interface %s\n", INTERFACE);
        return ERR_IPLINKSET_FAILED;
    }

    // Build the command for setting the interface up
    size = snprintf(command, 0, "/sbin/ifup %s", INTERFACE) + 1;

    command = realloc(command, size);

    // Error handling
    if(command == NULL){
        printf("Error could not reallocate memory\n");
        return ERR_MALLOC_FAILED;
    }

    snprintf(command, size, "/sbin/ifup %s", INTERFACE);

    // Bring the interface down for configuration
    if(system(command) == -1){
        printf("Error could not bring the interface %s up\n", INTERFACE);
        return ERR_IFUP_FAILED;
    }

    // Free the memory
    if(command != NULL){
        free(command);
    }

    return RET_E_OK;
}

int setupSocket(int *const socketFD, struct sockaddr_can *const addr){

    // Get the socket file descriptor for ioctl
    *socketFD = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    // Error handling
    if(*socketFD == -1){
        perror("Error getting socket file descriptor failed");
        return ERR_SOCKET_FAILED;
    }

    // Set the interace name in the ifr
    struct ifreq ifr;
    snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", INTERFACE);

    // Get the ifrindex of the interface name
    if(ioctl(*socketFD, SIOCGIFINDEX, &ifr) < 0){
        printf("Error could not get ifrindex: %s\n", strerror(errno));
        return ERR_IF_NOT_FOUND;
    }

    // Fill in the family and ifrindex
    addr->can_family  = AF_CAN;
    addr->can_ifindex = ifr.ifr_ifindex;

    // Bind to the socket
    if(bind(*socketFD, (struct sockaddr *) addr, sizeof(struct sockaddr_can)) != 0){
        perror("Error could not bind to the socket");
        close(*socketFD);
        return ERR_SETUP_FAILED;
    }

    // Check if CANFD frame support should be enabled
    if(CANFD){

        int can_raw_fd_frames = 1;
        if(setsockopt(*socketFD, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &can_raw_fd_frames, sizeof(can_raw_fd_frames)) != 0){
            perror("Error setsockopt CAN_RAW_FD_FRAMES failed");
            close(*socketFD);
            return ERR_SETSOCKOPT_FAILED;
        }

        printf("Enabled CANFD frames support on the socket\n");
    }

    return RET_E_OK;
}


/*******************************************************************************
 * END OF FILE
 ******************************************************************************/
