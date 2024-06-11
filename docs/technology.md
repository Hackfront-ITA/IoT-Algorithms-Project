# Technology

## High level diagram

![High level diagram](../res/high-level-diagram.png "High level diagram")

## Components

- [Nodes](#nodes), attached to critical points of the infrastructure to monitor
- [Controller](#controller), an edge device to aggregate data received from nodes, one for each infrastructure (eg. bridge)
- [Cloud system](#cloud-system), to store events received from the controllers and send alerts

### Nodes

- Accelerometer
- LoRa module
- Battery
- Solar panel

### Controller

- LoRa module
- Wi-Fi module

### Cloud system

- MQTT broker
- Database
- Listener service
