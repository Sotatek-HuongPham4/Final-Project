#ifndef __MMA8451Q_H_
#define __MMA8451Q_H_

#include "MKL46Z4.h"
#include "I2C.h"

// register addresses

#define MMA8451Q_STATUS            0x00
#define MMA8451Q_OUT_X_MSB           0x01
#define MMA8451Q_OUT_X_LSB           0x02
#define MMA8451Q_OUT_Y_MSB           0x03
#define MMA8451Q_OUT_Y_LSB           0x04
#define MMA8451Q_OUT_Z_MSB           0x05
#define MMA8451Q_OUT_Z_LSB           0x06
#define MMA8451Q_XYZ_DATA_CFG        0x0E
#define MMA8451Q_CTRL1               0x2A
#define MMA8451Q_CTRL2               0x2B
#define MMA8451Q_CTRL3               0x2C
#define MMA8451Q_CTRL4               0x2D
#define MMA8451Q_CTRL5               0x2E
#define MMA8451Q_OFF_X               0x2F
#define MMA8451Q_OFF_Y               0x30
#define MMA8451Q_OFF_Z               0x31

#define MMA8451Q_I2C_ADD (0x1D << 1)


typedef struct {
	float x;
	float y;
	float z;
} MMA8451Q_data;

void mma8451q_init(void);
void mma8451q_calib(void);
void mma8451_get_data(MMA8451Q_data* data);

#endif //__MMA8451Q_H_
