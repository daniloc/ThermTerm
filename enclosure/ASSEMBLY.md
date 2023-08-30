# Assembly

To build your own ThermTerm, follow along.

## Circuit

ThermTerm relies mostly on I2C devices, so you can wire them together however you want. If you'd like to use the included enclosure design, this sequence works best:

<img width="463" alt="Screenshot 2023-08-30 at 12 16 16 AM" src="https://github.com/daniloc/ThermTerm/assets/213358/47833da7-6bec-4de0-9e52-ff1e73b030c3">

By default, the infrared signal pin is `A0`, while the piezo signal is on `A1`. Change these in `_Constants.h` if needed. Oddly, the Feather only has a single ground pin on the board proper, so I snuck the piezo's ground wire into the IR blaster ground's solder blob. Seems to be fine.

You can affix these components to a piece of cardboard to test things out and ensure everything is doing what you expect.

## 3D printing

For something you can mount to the wall, a case design is provided. You'll want to orient all parts with their flattest faces against the print bed.

The design looks and works great using draft-quality layers at 0.3mm, but **you'll want to use PETG for the print**. While prototyping the case, I found PLA to be too brittle for the various posts that hold the components in place.

If your bridging is well tuned, you shouldn't need supports.

The knob/dial piece really should be printed with translucent PETG. This allows you to see the status light on the rotary encoder, and it's probably important for the IR signal to escape as well.

## Assembly

To use the included case design, wire up the components as described above. Sensors are held in place by slots, while posts hold the IR blaster, rotary encoder, and ESP32 board in place.

I affixed the small piezo buzzer to the metal RF shield on the ESP32 board to keep it out of the way.

The rotary encoder breakout gets a lot of impact, so I'd suggest anchoring it in place by melting the bottom two posts with a soldering iron.

To manage the StemmaQT cables, use the raceway molded into the base and tuck cables beneath components.