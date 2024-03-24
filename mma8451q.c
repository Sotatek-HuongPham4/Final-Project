#include "mma8451q.h"
#include <stdbool.h>

static void pause(void)
{
    int n;
    for(n=0; n<40; n++)
        __asm("nop");
//    for(n=0; n<100; n++);
}

static void mma8451q_set(uint8_t addr, uint8_t data)
{
	
	I2C0->C1 |=  I2C_C1_MST_MASK;
	I2C0->C1 |= I2C_C1_TX_MASK;
	
    WriteToI2C(MMA8451Q_I2C_ADD|I2C_WRITE); 
		I2CWait();
		I2C_Get_Ack();
	
    WriteToI2C(addr); 
		I2CWait();
		I2C_Get_Ack();
	
    WriteToI2C(data); 
		I2CWait();
		I2C_Get_Ack();
	
		
		I2C0->C1 &= ~I2C_C1_MST_MASK;
		I2C0->C1 &= ~I2C_C1_TX_MASK;
		pause();
}

void mma8451q_init(void)
{
	InitI2C(); 
	
	unsigned char reg_status = 0;
    mma8451q_set(MMA8451Q_CTRL2, 0x40); 
    do  
    {
       reg_status = (mma8451q_read(MMA8451Q_CTRL2) & 0x40);
    }
	while (reg_status);
		
    mma8451q_set(MMA8451Q_XYZ_DATA_CFG, 0x00); 
    mma8451q_set(MMA8451Q_CTRL2, 0x02);        

    mma8451q_set(MMA8451Q_CTRL1, 0x11);        
}


signed short mma8451q_read(uint8_t addr)
{
		uint8_t result;
		
		I2C0->C1  |=  I2C_C1_MST_MASK;
		I2C0->C1 |= I2C_C1_TX_MASK;
    WriteToI2C(MMA8451Q_I2C_ADD|I2C_WRITE); 
	
		I2CWait();
		I2C_Get_Ack();
	
	  WriteToI2C(addr); 
		I2CWait();
		I2C_Get_Ack();
	
		I2C0->C1 |= 0x04;
		WriteToI2C(MMA8451Q_I2C_ADD|I2C_READ);
		I2CWait();
		I2C_Get_Ack();
	
		I2C0->C1 &= ~I2C_C1_TX_MASK;
		I2C0->C1 |= I2C_C1_TXAK_MASK;
		result = I2C0->D;
		I2CWait();
		
		// Slave & RX mode
		I2C0->C1  &= ~I2C_C1_MST_MASK;
		I2C0->C1 &= ~I2C_C1_TX_MASK;
		result = I2C0->D;
		pause();
    return result;
}

void mma8451q_calib(void)
{
	unsigned char reg_status = 0;
	float x_offset, y_offset, z_offset;
	int16_t x_read, y_read, z_read;

    if (!reg_status)           // Wait for a first set of data         
    {
		reg_status = (mma8451q_read(MMA8451Q_STATUS) & 0x08);
    }

	x_read = ((int16_t) (mma8451q_read(MMA8451Q_OUT_X_MSB)<<8 | mma8451q_read(MMA8451Q_OUT_X_LSB))) >> 2;
	y_read = ((int16_t) (mma8451q_read(MMA8451Q_OUT_Y_MSB)<<8 | mma8451q_read(MMA8451Q_OUT_Y_LSB))) >> 2;
	z_read = ((int16_t) (mma8451q_read(MMA8451Q_OUT_Z_MSB)<<8 | mma8451q_read(MMA8451Q_OUT_Z_LSB))) >> 2;

    x_offset = x_read / 8 * (-1) - 1;  				
    y_offset = y_read / 8 * (-1) - 1; 				
    z_offset = (z_read - 4096) / 8 * (-1) - 1; 	

    mma8451q_set(MMA8451Q_CTRL1, 0x10); 

    mma8451q_set(MMA8451Q_OFF_X, x_offset);	
    mma8451q_set(MMA8451Q_OFF_Y, y_offset);	
    mma8451q_set(MMA8451Q_OFF_Z, z_offset);	
	
	mma8451q_set(MMA8451Q_CTRL1, 0x11);  	
		
}

void mma8451_get_data(MMA8451Q_data* data)
{
	int16_t x_out, y_out, z_out;
		x_out = ((int16_t) (mma8451q_read(MMA8451Q_OUT_X_MSB)<<8 | mma8451q_read(MMA8451Q_OUT_X_LSB))) >> 2;
		y_out = ((int16_t) (mma8451q_read(MMA8451Q_OUT_Y_MSB)<<8 | mma8451q_read(MMA8451Q_OUT_Y_LSB))) >> 2;
		z_out = ((int16_t) (mma8451q_read(MMA8451Q_OUT_Z_MSB)<<8 | mma8451q_read(MMA8451Q_OUT_Z_LSB))) >> 2;

		data->x = ((float) x_out) / 4096;
		data->y = ((float) y_out) / 4096;
		data->z = ((float) z_out) / 4096;
}