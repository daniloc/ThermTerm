# Assembly

To build your own ThermTerm, follow along.

## Circuit

ThermTerm relies mostly on I2C devices, so you can connect them together in whatever order you want. You can affix these components to a piece of cardboard to test things out and ensure everything is doing what you expect.

![264531564-6f04c1c9-9900-47f6-958d-a4b7f66801d4](https://github.com/daniloc/ThermTerm/assets/213358/4cb90d85-3e45-42c0-bec7-e6b289ea0d4a)


If you'd like to use the included enclosure design, this sequence works best:

<img width="326" alt="Circuit diagram" src="https://github.com/daniloc/ThermTerm/assets/213358/e8f3b935-a177-4b0c-bfd6-19c4f76ecea5">

By default, the infrared signal pin is `A0`, while the piezo signal is on `A1`. Change these in `_Constants.h` if needed. Oddly, the Feather only has a single ground pin on the board proper, so I snuck the piezo's ground wire into the IR blaster ground's solder blob. Seems to be fine.

## 3D printing

For something you can mount to the wall, a case design is provided. You'll want to orient all parts with their flattest faces against the print bed.

<img width="1081" alt="Printing layout example" src="https://github.com/daniloc/ThermTerm/assets/213358/c54829f6-e97c-4868-b15c-bbc599a703d2">

The design looks and works great using draft-quality layers at 0.3mm, but **you'll want to use PETG for the print**. While prototyping the case, I found PLA to be too brittle for the various posts that hold the components in place.

If your bridging is well tuned, you shouldn't need supports.

The knob/dial piece really should be printed with translucent PETG. This allows you to see the status light on the rotary encoder, and it's probably important for the IR signal to escape as well.

## Assembly

To use the included case design, wire up the components as described above. Sensors are held in place by slots, while posts hold the IR blaster, rotary encoder, and ESP32 board in place.

<img height=400 src="https://github.com/daniloc/ThermTerm/assets/213358/abd60699-070d-47b1-a586-06f9e2527edb">

<img height=400 src="https://github.com/daniloc/ThermTerm/assets/213358/94811e0e-3148-46a2-a6f9-cef78c4bb0bc">

I affixed the small piezo buzzer to the metal RF shield on the ESP32 board to keep it out of the way.

<img height=400 src="https://github.com/daniloc/ThermTerm/assets/213358/9361b272-a131-4508-b8f4-ddafc732e228">

The rotary encoder breakout gets a lot of impact, so I'd suggest anchoring it in place by melting the bottom two posts with a soldering iron.

<img height=400 src="https://github.com/daniloc/ThermTerm/assets/213358/acac3e5b-a52b-4fb7-a2b4-66b2eccf2e02">

To manage the StemmaQT cables, use the raceway molded into the base to tuck the longest cable run, from ESP32 to the light sensor. For the shorter cables, fold them gently against the case to prevent the dial scraping against them.

<img height=400 src="https://github.com/daniloc/ThermTerm/assets/213358/fb3d93bd-fd86-4021-bb09-79ca59367cbc">

If desired, print the key caps and press-fit them into the rectangular recesses on the buttons in front of the case. I found it helped to remove the front face, position the key caps as best I could manage, then place the shell face down and gently press against the insides of the button tabs with the rubberized handle-end of some pliers. This clicked the keycaps into place without bending the tabs.

<img height=400 src="https://github.com/daniloc/ThermTerm/assets/213358/8ab89ca5-caff-4ade-ae6b-ba7d81baf312">

Pop on the dial, and you're done.
