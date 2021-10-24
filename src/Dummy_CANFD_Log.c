/*******************************************************************************
 \project   INFM_HIL_Interface
 \file      Dummy_CANFD_Log.c
 \brief     Provides functions for the logging.
 \author    Matthias Bank
 \version   1.0.0
 \date      24.10.2021
 ******************************************************************************/


/*******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "Dummy_CANFD_Config.h"
#include "Dummy_CANFD_Log.h"


/*******************************************************************************
 * FUNCTION DEFINITIONS
 ******************************************************************************/

void logElapsedTime(FILE *const file, struct timespec const *const startCycleTime, struct timespec const *const stopCycleTime){

    // Calculate the difference between timestamp components
    long long diffSec  = stopCycleTime->tv_sec  - startCycleTime->tv_sec;
    long long diffNsec = stopCycleTime->tv_nsec - startCycleTime->tv_nsec;

    // Convert second difference to nanoseconds
    long long diffSec2Nsec = diffSec * 1000000000;

    // Calculate elapsed time in nanoseconds
    long long diff = diffSec2Nsec + diffNsec;
    fprintf(file, "%lld\n", diff);

    // Print information
    if(VERBOSE){
        printf("Elapsed time: %lld nsec \n\n", diff);
    }
}


void logRunInformation(FILE *const file, struct timespec const *const startTime, struct timespec const *const stopTime){

    fprintf(file, "### Run Information ###\n");

    // Write the interface into the log
    fprintf(file, "%s\n", INTERFACE);

    // Write the mode into the log
    fprintf(file, "%d\n", CANFD);

    // Write the bitrate into the log
    fprintf(file, "%d\n", BITRATE);

    // Write the sample point into the log
    fprintf(file, "%.3f\n", SAMPLEPOINT);

    // Write the dbitrate into the log
    fprintf(file, "%d\n", DBITRATE);

    // Write the dsample point into the log
    fprintf(file, "%.3f\n", DSAMPLEPOINT);

    // Write the message size into the log
    fprintf(file, "%d\n", PAYLOADSIZE);

    // Write the amount of cycles into the log
    fprintf(file, "%d\n", MAXCYCLES);

    // Calculate and log the total time of all cycles in seconds
    long diffSec  = stopTime->tv_sec  - startTime->tv_sec;
    fprintf(file, "%ld\n", diffSec);

    // Print information
    printf("### Run Information: ### \n");
    printf("Interface    : %s        \n", INTERFACE);
    printf("Mode         : %d        \n", CANFD);
    printf("Bitrate      : %d baud   \n", BITRATE);
    printf("Sample point : %.3f      \n", SAMPLEPOINT);
    printf("DBitrate     : %d baud   \n", DBITRATE);
    printf("DSample point: %.3f      \n", DSAMPLEPOINT);
    printf("Payload size : %d bytes  \n", PAYLOADSIZE);
    printf("Total cycles : %d cycles \n", MAXCYCLES);
    printf("Total time   : %ld sec   \n", diffSec);
}


/*******************************************************************************
 * END OF FILE
 ******************************************************************************/