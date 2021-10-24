/*******************************************************************************
 \project   INFM_HIL_Interface
 \file      Dummy_CANFD_Log.h
 \brief     This header provides functions for the logging.
 \author    Matthias Bank
 \version   1.0.0
 \date      24.10.2021
 ******************************************************************************/
#ifndef DUMMY_CANFD_LOG_H
#define DUMMY_CANFD_LOG_H


/*******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include <stdio.h>
#include <time.h>


/*******************************************************************************
 * FUNCTION DECLARATIONS
 ******************************************************************************/

/**
 * Calculates the elapsed time between to clock monotonic timestamps in nano seconds
 * and writes the result into a file
 *
 * @param file            - Writable file in which the elapsed time will be logged
 * @param startCycleTime  - The first timestamp
 * @param stopCycleTime   - The second timestamp
 */
extern void logElapsedTime(FILE *const file, struct timespec const *const startCycleTime, struct timespec const *const stopCycleTime);


/**
 * Logs general information about the run into a file
 *
 * @param file        - Writable file in which the information will be logged
 * @param startTime   - The first timestamp
 * @param stopTime    - The second timestamp
 */
extern void logRunInformation(FILE *const file, struct timespec const *const startTime, struct timespec const *const stopTime);


#endif //DUMMY_CANFD_LOG_H


/*******************************************************************************
 * END OF FILE
 ******************************************************************************/
