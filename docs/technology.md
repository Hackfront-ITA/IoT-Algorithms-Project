# Technology

## High level diagram

![High level diagram](../res/high-level-diagram.png "High level diagram")

## Components

- [Nodes](#nodes), attached to critical points of the infrastructure to monitor
- [Controller](#controller), an edge device to aggregate data received from nodes, one for each infrastructure (eg. bridge)
- [Cloud system](#cloud-system), to store events received from the controllers and send alerts

### Nodes

Each node is composed of:

- Heltec LoRa V3 board
- [Accelerometer](#accelerometer) (Analog Devices ADXL345)
- [LoRa module](#lora-module) (Semtech SX1262)
- [Battery](#battery) (3.7V Li-Po)
- Solar panel

Each node samples the three axes of an accelerometer at a rate of 200 Hz, sufficient to capture vibrations at a maximum of 100 Hz (Nyquist frequency).
After collecting a window of samples (eg. 20s), some calculations are done for each axis:

- FFT to extract frequency components
- Z-score to find outliers
- Clustering for neighboring values

Then, resulting values are sent to the [controller](#controller) via LoRa radio.

#### Accelerometer

The accelerometer is the Analog Devices ADXL345. It is connected to the Heltec board using the I2C bus:

| ADXL345 | Heltec                 |
| :------ | :--------------------- |
| VCC     | 3.3V                   |
| GND     | GND                    |
| SDA     | GPIO 47 (pin 13 right) |
| SCL     | GPIO 48 (pin 14 right) |
| CS      | 3.3V                   |
| SDO     | GND                    |

CS to 3.3V is needed to put the accelerometer in I2C mode, as it can use either I2C or SPI.

SDO to GND is needed to signal the accelerometer to use the main I2C address (0xE5).

#### Battery

The Heltec board provides also a voltage regulator and a BMS (battery management system) with a dedicated 2-pin connector for a Li-Ion/Li-Po battery.

This allows the board to operate in the absence of an external power source until the battery runs out.
When the power source is restored, the BMS transparently begins recharging the battery until it is fully charged, and then stops drawing current.

This setup is ideal for our scenario, where a solar panel is used to power the node, but in the absence of sun, the battery should continue to provide power.

Connections:

- The battery is connected to the specific connector on the Heltec board
- The solar panel output (+/-) is connected to 5V/GND of the Heltec board

### Controller

Each controller is composed of:

- Heltec LoRa V3 board
- [LoRa module](#lora-module)
- Wi-Fi module

#### LoRa module

LoRa module is the Semtech SX1262. It is provided by the Heltec board and is internally connected to SPI2 bus.
The connections to use are the following:

| SX1262  | ESP32   |
| :------ | :------ |
| MISO    | GPIO 11 |
| MOSI    | GPIO 10 |
| SCLK    | GPIO  9 |
| NSS     | GPIO  8 |
| RST     | GPIO 12 |
| BUSY    | GPIO 13 |

LoRa parameters (spreading factor, bandwidth, coding rate) are adjusted to meet range requirements (at least 100m). This is explained in detail in the section [LoRa parameters](./evaluation.md#lora-parameters) section of the evaluation document.

### Cloud system

- MQTT broker
- Database
- Listener service
