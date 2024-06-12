# Concept

This project aims to provide a tool for performing what is known as Structural
Health Monitoring (SHM). It is an area of application in which IoT devices are
already largely used. One of the most common way of doing SHM is to measure
accelerations and vibrations of the structures, in order study their natural
frequencies. A change in their natural frequency could potentially signal a
damage in the structure.

Our project primarily offers a way to measure such vibration and movements.

### Functional details and requirements

The idea is to have multiple devices (nodes) attached to the structure,
equipped with accelerometers for measuring vibrations.

The vibration values we are interested in are no larger than 100Hz. The desired resolution
of the acceleration measurements is in the order of micro g, however the accelerometer
we used provides a resolution in the order of milli g that should still suffice.

These nodes then send data to an edge device using LoRa, a long range wireless
transmission protocol. Such protocol is needed since the nodes could easily be
outside the range of other wireless trasmission protocols such as WiFi and
Bluetooth.

The edge devices then translates LoRa packets into MQTT messages that are then
sent via WiFi to a cloud database, making possible an analysis of vibration values
over long time periods.

These devices and should be battery powered for autonomous operation and ease of use.
Using a solar panel for recharging the batteries should be used to significantly reduce
the need of manteinance and batteries of large capacity.

Since these device effectively provide a way to measure vibrations in real time,
they could also be used to check for dangerous vibrations and movement for the safety
of the users of the structure.

For further technical details please check the [technology](./technology.md) and [evaluation](./evaluation.md)
documents.
