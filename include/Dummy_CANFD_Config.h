/*******************************************************************************
 \project   INFM_HIL_Interface
 \file      Dummy_CANFD_Config.h
 \brief     Defines the adjustable communication parameters.
 \author    Matthias Bank
 \version   1.0.0
 \date      24.10.2021
 ******************************************************************************/
#ifndef DUMMY_CANFD_CONFIG_H
#define DUMMY_CANFD_CONFIG_H

/** @file */


/*******************************************************************************
 * DEFINES
 ******************************************************************************/

/** The changed ID of the frames we are sending back. */
#define FRAMEID      0x222      

/** Flag for enabling CANFD support. */
#define CANFD        0         

/** The name of the interface that should be configured. */
#define INTERFACE    "vcan0"

/** Enables printing information during the recv-send loop. */
#define VERBOSE      1


#endif //DUMMY_CANFD_CONFIG_H


/*******************************************************************************
 * END OF FILE
 ******************************************************************************/