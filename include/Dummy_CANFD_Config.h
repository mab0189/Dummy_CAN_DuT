/*******************************************************************************
 \project   INFM_HIL_Interface
 \file      Dummy_CANFD_Config.h
 \brief     Defines the adjustable communication parameters.
 \author    Matthias Bank
 \version   1.0.0
 \date      24.10.2021
 ******************************************************************************/
#ifndef DUMMY_CAN_DUT_DUMMY_CANFD_CONFIG_H
#define DUMMY_CAN_DUT_DUMMY_CANFD_CONFIG_H


/*******************************************************************************
 * DEFINES
 ******************************************************************************/
#define PAYLOADSIZE  8          // The payload size of the frame CAN (0-8) CANFD (0-64)
#define MAXCYCLES    100        // The amount of cycles that should be done

#define CANFD        1          // Flag for enabling CANFD
#define INTERFACE    "can0"     // The name of the interface that should be configured

#define BITRATE      500000     // The bitrate value in baud
#define SAMPLEPOINT  0.700      // The sample point value

#define DBITRATE     2000000    // The dbitrate value in baud
#define DSAMPLEPOINT 0.800      // The dsample point value

#define VERBOSE      1          // Enables printing information during the send-recv cycle


#endif //DUMMY_CAN_DUT_DUMMY_CANFD_CONFIG_H


/*******************************************************************************
 * END OF FILE
 ******************************************************************************/