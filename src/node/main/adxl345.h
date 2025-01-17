#ifndef N_ADXL_345_H
#define N_ADXL_345_H

#define ADXL345_REG_DEVID           0x00  // Device ID
// 0x01 to 0x1C are reserved registers
#define ADXL345_REG_THRESH_TAP      0x1D  // Tap threshold
#define ADXL345_REG_OFSX            0x1E  // X-axis offset
#define ADXL345_REG_OFSY            0x1F  // Y-axis offset
#define ADXL345_REG_OFSZ            0x20  // Z-axis offset
#define ADXL345_REG_DUR             0x21  // Tap duration
#define ADXL345_REG_LATENT          0x22  // Tap latency
#define ADXL345_REG_WINDOW          0x23  // Tap window
#define ADXL345_REG_THRESH_ACT      0x24  // Activity threshold
#define ADXL345_REG_THRESH_INACT    0x25  // Inactivity threshold
#define ADXL345_REG_TIME_INACT      0x26  // Inactivity time
#define ADXL345_REG_ACT_INACT_CTL   0x27  // Axis enable control for activity and inactivity detection
#define ADXL345_REG_THRESH_FF       0x28  // Free-fall threshold
#define ADXL345_REG_TIME_FF         0x29  // Free-fall time
#define ADXL345_REG_TAP_AXES        0x2A  // Axis control for single/double tap
#define ADXL345_REG_ACT_TAP_STATUS  0x2B  // Source for single/double tap
#define ADXL345_REG_BW_RATE         0x2C  // Data rate and power mode control
#define ADXL345_REG_POWER_CTL       0x2D  // Power-saving features control
#define ADXL345_REG_INT_ENABLE      0x2E  // Interrupt enable control
#define ADXL345_REG_INT_MAP         0x2F  // Interrupt mapping control
#define ADXL345_REG_INT_SOURCE      0x30  // Source of interrupts
#define ADXL345_REG_DATA_FORMAT     0x31  // Data format control
#define ADXL345_REG_DATA            0x32  // Data
#define ADXL345_REG_DATAX0          0x32  // X-axis data 0
#define ADXL345_REG_DATAX1          0x33  // X-axis data 1
#define ADXL345_REG_DATAY0          0x34  // Y-axis data 0
#define ADXL345_REG_DATAY1          0x35  // Y-axis data 1
#define ADXL345_REG_DATAZ0          0x36  // Z-axis data 0
#define ADXL345_REG_DATAZ1          0x37  // Z-axis data 1
#define ADXL345_REG_FIFO_CTL        0x38  // FIFO control
#define ADXL345_REG_FIFO_STATUS     0x39  // FIFO status

#define ADXL345_OUTPUT_DATA_RATE_200 0x0B
#define ADXL345_OUTPUT_DATA_RATE_400 0x0C
#define ADXL345_OUTPUT_DATA_RATE_800 0x0D
#define ADXL345_OUTPUT_DATA_RATE_1600 0x0E

#define ADXL345_FIFO_SIZE 32

typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
} adxl345_raw_data_t;

typedef struct {
	float x;
	float y;
	float z;
} adxl345_g_data_t;

esp_err_t adxl345_init(void);
esp_err_t adxl345_read_data(adxl345_g_data_t *value);

#endif /* end of include guard: N_ADXL_345_H */
