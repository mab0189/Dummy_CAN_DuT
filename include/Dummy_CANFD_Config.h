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


/*******************************************************************************
 * DEFINES
 ******************************************************************************/
#define FRAMEID      0x222      // The changed ID of the frames we are sending back

#define CANFD        1          // Flag for enabling CANFD
#define INTERFACE    "can0"     // The name of the interface that should be configured

#define BITRATE      500000     // The bitrate value in baud
#define SAMPLEPOINT  0.700      // The sample point value

#define DBITRATE     2000000    // The dbitrate value in baud
#define DSAMPLEPOINT 0.800      // The dsample point value

#define VERBOSE      1          // Enables printing information during the send-recv cycle


#endif //DUMMY_CANFD_CONFIG_H


/*******************************************************************************
 * END OF FILE
 ******************************************************************************/