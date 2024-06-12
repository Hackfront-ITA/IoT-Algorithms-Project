# Evaluation

## Contents
- [Power consumption](#energy-consumption)
- [Energy harvesting](#energy-harvesting)
- [LoRa parameters](#lora-parameters)
- [Task timing](#task-timing)
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

### Task timing

The time-critical parts of the node firmware are the data_collect and data_process tasks.

The data_collect task fills a array containing 20 seconds worth of samples. Once finished, it wakes up the data_process task, which takes the filled array, processes the data and sends the results via LoRa.

The data_collect task does not wait the completion of data_process task, otherwise the samples could be lost, so it starts a new sampling window in another *slot* of the data array.

As there are two slots, the data_process task has to release the old slot before the data_collect starts collecting again there.

It is critical that an iteration of the data_process task takes less than an iteration of the data_collect task.

A measurement is done to avoid this scenario. With `esp_timer_get_time()` function, we measured the time taken by an iteration of data_process task.

The calculation part of the task has a fixed duration, on average 48 ms.
For each event sent via LoRa, additional 75.5 ms are spent, which is roughly the airtime of the packet.

With a sampling window duration of 20 seconds, the maximum number of packets sent is:

$$n_{pkt} = \frac{20000 ms - 48 ms}{75.5 ms} = 264 $$

which is a lot more than we need, so there is no timing issue between tasks.

### Time sync

As our node network is distributed, a method to sync events that happen concurrently is needed.

It can happen that a node has just finished doing FFT + z-score calculation but another node just started sampling.

How do we aggregate values belonging to unaligned windows?

When each node transmits an event packet, it attaches an *epoch* value.
This value is managed by the controller, which increments it and broadcasts via LoRa to all the nodes the new epoch each 40 seconds, which is double of the sampling window.

This ensures that the values are aggregated inside a window of 40 seconds, which is a bit less than the minute of end-to-end latency requirement stated before.

### LoRa parameters
We used a tool ([LoRaTools air time calculator](https://www.loratools.nl/#/airtime)) to estimate time on air and [this guide](https://medium.com/home-wireless/testing-lora-radios-with-the-limesdr-mini-part-2-37fa481217ff).
