/*******************************************************************************
 \project   INFM_HIL_Interface
 \file      Dummy_CANFD_DuT.c
 \brief     This application configures a CAN/CANFD interface before creating a
            raw CAN/CANFD socket through socketCAN. The dummy is a simple echo
            server that sends the received CAN/CANFD frames back with a
            different ID.
 \author    Matthias Bank
 \version   1.0.0
 \date      24.10.2021
 ******************************************************************************/


/*******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "Dummy_CANFD_Config.h"
#include "Dummy_CANFD_Log.h"
#include "Dummy_CANFD_Socket.h"
#include <linux/can/raw.h>
#include <linux/can.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>


/*******************************************************************************
 * DEFINES
 ******************************************************************************/
#define FRAMEID 0x222                                           // The ID of the frame we are sending
#define LOGFILE "/var/log/benchmark/canfd_bench_server.log"     // The path to the log file


/*******************************************************************************
 * FUNCTION DEFINITIONS
 ******************************************************************************/

/**
 * Handles the shutdown procedure
 *
 * @param retCode  - The return code
 * @param frame    - Storage for the generated frame
 * @param socketFD - The socket file descriptor
 * @param file     - The file descriptor
 */
int shutdownHandler(int retCode, void *frame, int socketFD, FILE *file){

    // Free the frame memory
    if(frame != NULL){
        free(frame);
    }

    // Close the socket
    if(socketFD != -1){
        close(socketFD);
    }

    // Close the file
    if(file != NULL){
        fclose(file);
    }

    exit(retCode);
}

int main(){

    int counter = 0;                                // The cycle counter

    struct timespec startTime;                      // Start timestamp of all cycles
    struct timespec stopTime;                       // Stop  timestamp of all cycles

    struct timespec startCycleTime;                 // Start timestamp of the send-recv cycle
    struct timespec stopCycleTime;                  // Stop  timestamp of the send-recv cycle

    int socketFD = -1;                              // Our socket file descriptor
    struct sockaddr_can socketAddr;                 // Our socket address

    int numbytes;                                   // Number of bytes that we send or received
    char buf[CANFD_MTU + 1];                        // Buffer for storing the received message payload

    int recvID;                                     // The ID of the received frame
    void *frame = NULL;                             // Pointer to the frame that is being sent
    size_t frameSize;                               // The size of the frame that is being received
    int recvPayloadSize;                            // The size of the payload of the received frame


    // Create an empty file to log the elapsed time
    FILE *file = fopen(LOGFILE, "w+");

    // Error handling
    if(file == NULL){
        perror("Error logfile could not be opened to write");
        shutdownHandler(ERR_LOG_WRITE, frame, socketFD, file);
    }

    // Prepare the CAN/CANFD interface
    if(prepareCanInterface() != 0){
        printf("Error could not prepare the interface %s\n", INTERFACE);
        shutdownHandler(ERR_SETUP_FAILED, frame, socketFD, file);
    }

    printf("Finished preparing the interface %s\n", INTERFACE);

    // Set up the socket
    if(setupSocket(&socketFD, &socketAddr) != 0){
        printf("Error could not setup the socket \n");
        shutdownHandler(ERR_SETUP_FAILED, frame, socketFD, file);
    }

    printf("Setup the socket on the interface %s\n", INTERFACE);

    // Check which mode is selected and set values accordingly
    if(CANFD){
        frameSize = sizeof(struct canfd_frame);
        frame = malloc(frameSize);
    }else{
        frameSize = sizeof(struct can_frame);
        frame = malloc(frameSize);
    }

    // Error handling
    if(frame == NULL){
        printf("Error could not allocate memory \n");
        shutdownHandler(ERR_MALLOC_FAILED, frame, socketFD, file);
    }

    // Send Receive Loop
    printf("\n### Starting receive-send loop: ### \n\n");

    // Get the start timestamp
    if(clock_gettime(CLOCK_MONOTONIC, &startTime) == -1){
        perror("Error getting the start time failed");
        shutdownHandler(ERR_CLOCK_GETTIME, frame, socketFD, file);
    }

    while(counter < MAXCYCLES){

        if(VERBOSE){
            printf("Blocking on receive!\n");
        }

        // Receive data from the socket
        numbytes = recv(socketFD, frame, frameSize, 0);

        // Error handling:
        if(numbytes == -1){
            perror("Error recv failed");
            shutdownHandler(ERR_RECV_FAILED, frame, socketFD, file);
        }else if(numbytes != frameSize){
            printf("Error received bytes are not equal to the frame size");
            shutdownHandler(ERR_RECV_FAILED, frame, socketFD, file);
        }

        // Get the ID and set the new ID
        if(CANFD){
            struct canfd_frame *canfdFrame = (struct canfd_frame*) frame;
            recvID = canfdFrame->can_id;
            canfdFrame->can_id = FRAMEID;
        }else{
            struct can_frame *canFrame = (struct can_frame*) frame;
            recvID = canFrame->can_id;
            canFrame->can_id = FRAMEID;
        }

        if(VERBOSE){

            // Get the payload and payload size
            if(CANFD){
                struct canfd_frame *canfdFrame = (struct canfd_frame*) frame;
                recvPayloadSize = canfdFrame->len;
                memcpy(buf, canfdFrame->data, canfdFrame->len);
            }else{
                struct can_frame *canFrame = (struct can_frame*) frame;
                recvPayloadSize = canFrame->can_dlc;
                memcpy(buf, canFrame->data, canFrame->can_dlc);
            }

            buf[recvPayloadSize] = '\0';
            printf("Got %d bytes total with ID 0x%x and %d bytes payload:\n\"%s\"\n",
                   numbytes, recvID, recvPayloadSize, buf);
        }

        // Get the start cycle timestamp
        if(clock_gettime(CLOCK_MONOTONIC, &startCycleTime) == -1){
            shutdownHandler(ERR_CLOCK_GETTIME, frame, socketFD, file);
            perror("Error getting the start cycle time failed");
        }

        // Send the frame
        numbytes = send(socketFD, frame, frameSize, 0);

        // Error handling:
        if(numbytes == -1){
            perror("Error sendto failed");
            shutdownHandler(ERR_SEND_FAILED, frame, socketFD, file);
        }else if(numbytes != frameSize){
            printf("Error send bytes are not equal to the frame size");
            shutdownHandler(ERR_SEND_FAILED, frame, socketFD, file);
        }

        if(VERBOSE){
            printf("Sent %d bytes total with ID 0x%x and %d bytes payload:\n\"%s\"\n",
                   numbytes, FRAMEID, recvPayloadSize, buf);
        }

        // Get the stop cycle timestamp
        if(clock_gettime(CLOCK_MONOTONIC, &stopCycleTime) == -1){
            perror("Error getting the stop cycle time failed");
            shutdownHandler(ERR_CLOCK_GETTIME, frame, socketFD, file);
        }

        // Log the elapsed time and increase the counter
        logElapsedTime(file, &startCycleTime, &stopCycleTime);
        counter++;
    }

    // Get the stop timestamp
    if(clock_gettime(CLOCK_MONOTONIC, &stopTime) == -1){
        perror("Error getting the stop time failed");
        shutdownHandler(ERR_CLOCK_GETTIME, frame, socketFD, file);
    }

    // Log the run information
    logRunInformation(file, &startTime, &stopTime);

    // Call the shutdown handler
    shutdownHandler(RET_E_OK, frame, socketFD, file);
    return RET_E_OK;
}


/*******************************************************************************
 * END OF FILE
 ******************************************************************************/
