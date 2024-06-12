# Evaluation

## Contents
- [Power consumption](#energy-consumption)
- [Energy harvesting](#energy-harvesting)
- [LoRa parameters](#lora-parameters)
- [Time sync](#time-sync)

### Power consumption


### Energy harvesting

Based on the previous section, the average power consumption is xxx.xx mW.

In order to ensure the system can operate on battery power for an entire day, we need to calculate the battery capacity:

$$ E_{bat} = P_{avg} \cdot t_{day} = xxx.xx mW \cdot 24 h = xxxx.xx mWh = x.xxx Wh $$

A battery recovered from a tablet, with a capacity of $11.1 Wh$ should be sufficient to meet the energy needs for a bit more than a day.

Regarding the solar panel, it should provide for a day enough energy to power the node AND recharge the battery.
The battery may be almost empty and it needs to be recharged during the period of light to be ready for the next day.

So the calculation of the solar panel power, assuming 6 hours of light a day ($t_{light} = 6 h$) is:

$$ P_{solar} = \frac{E_{bat}}{t_{light}} + P_{avg} = \frac{x.xxx Wh}{6 h} + x.xxx Wh = xx.xx W $$

A solar panel like [this one](https://www.amazon.it/s?k=pannello+solare+5v) on Amazon should be sufficient.

### Time sync

As our node network is distributed, a method to sync events that happen concurrently is needed.

It can happen that

### LoRa parameters
We used a tool ([LoRaTools air time calculator](https://www.loratools.nl/#/airtime)) to estimate time on air and [this guide](https://medium.com/home-wireless/testing-lora-radios-with-the-limesdr-mini-part-2-37fa481217ff).
