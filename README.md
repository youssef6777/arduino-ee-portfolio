# Water Level Sensor

Monitors water level in real time using a resistive sensor. Three LEDs indicate
the current state and a passive buzzer fires an alarm when the level is critically high.

## Demo

<!-- Add your GIF here -->
![Water level demo](images/demo.gif)

<!-- Drag and drop your video file into the GitHub README editor to embed it -->

## Photos

![Top-down view](images/circuit1.jpg)

![Circuit overview](images/circuit2.jpg)

## How It Works

1. A resistive water sensor outputs an analog voltage proportional to how much of
   its surface is submerged — more contact with water means lower resistance and
   higher voltage at the signal pin
2. `analogRead()` converts that voltage to a value between 0 and 1023
3. The value is compared against two thresholds to determine one of three states
4. The corresponding LED lights up; if the level is critically high the buzzer
   pulses a 2500 Hz alarm beep

| State | Condition | LED | Buzzer |
|-------|-----------|-----|--------|
| Low | Below 250 | 🟢 Green | Silent |
| Medium | 250 – 300 | 🟡 Yellow | 1000 Hz warning beep |
| High | Above 300 | 🔴 Red | 2500 Hz alarm beep |

## Components Used

- 1x Arduino Uno (Elegoo)
- 1x Resistive water level sensor
- 1x Red LED
- 1x Yellow LED
- 1x Green LED
- 1x Passive buzzer
- 3x 220Ω resistors
- Breadboard + jumper wires

## Circuit Wiring

| Component | Arduino Pin |
|-----------|-------------|
| Sensor signal | A0 |
| Red LED | 9 |
| Yellow LED | 10 |
| Green LED | 11 |
| Buzzer | 8 |

## What I Learned

- **Analog input and ADC** — `analogRead()` converts a continuous voltage (0–5V)
  into a discrete integer (0–1023). The water sensor works by changing its resistance
  based on how much of the probe is submerged, which changes the voltage at A0.

- **Threshold-based state machines** — rather than trying to calculate an exact
  depth (which is unreliable with resistive sensors due to non-linearity), the
  design uses two thresholds to define three zones. This is a common pattern in
  real embedded systems where sensor precision is limited.

- **`tone()` and `noTone()`** — `tone(pin, frequency, duration)` drives a passive
  buzzer at a specific frequency without blocking the loop. A passive buzzer requires
  the microcontroller to generate the waveform, unlike an active buzzer which has
  a built-in oscillator and only needs `digitalWrite()`.

- **Sensor calibration** — the thresholds (250/300) are not universal. They were
  determined by submerging the sensor at different depths and observing raw values
  in Serial Monitor. The same sensor in different water (tap vs. distilled) will
  read differently due to varying conductivity.

## Challenges

Red and yellow LEDs were flickering rapidly at the boundary between states even
when the sensor was held still. Opened Serial Monitor and saw the raw readings
oscillating between 248 and 252 — right on the `low_water` threshold of 250.
A single noisy reading was enough to flip the state on every loop cycle.
Identified this as electrical noise inherent to resistive sensors in water.
Fixed by tightening the threshold gap and adding a `delay()` to slow the read
rate, which reduced how often a stray reading could trigger a state change.

## Next Steps

- [ ] Average multiple readings per loop cycle to smooth out noise more reliably
- [ ] Power the sensor through a digital pin to prevent electrolysis corrosion
      from continuous current flow through the water
- [ ] Add Python serial logging to save timestamped readings to a CSV file
- [ ] Plot water level over time using matplotlib
