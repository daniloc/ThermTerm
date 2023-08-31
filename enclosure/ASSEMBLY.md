# Assembly

To build your own ThermTerm, follow along.

## Circuit

ThermTerm relies mostly on I2C devices, so you can connect them together in whatever order you want. If you'd like to use the included enclosure design, this sequence works best:

<img width="326" alt="Screenshot 2023-08-30 at 11 41 31 PM" src="https://github.com/daniloc/ThermTerm/assets/213358/0dcd5ed1-f152-4741-85c4-ed8c4cd35101">

By default, the infrared signal pin is `A0`, while the piezo signal is on `A1`. Change these in `_Constants.h` if needed. Oddly, the Feather only has a single ground pin on the board proper, so I snuck the piezo's ground wire into the IR blaster ground's solder blob. Seems to be fine.

You can affix these components to a piece of cardboard to test things out and ensure everything is doing what you expect.

## 3D printing

For something you can mount to the wall, a case design is provided. You'll want to orient all parts with their flattest faces against the print bed.

<img width="1081" alt="Screenshot 2023-08-31 at 12 02 12 AM" src="https://github.com/daniloc/ThermTerm/assets/213358/98896f95-0a88-45fc-a6c2-cb56f115571c">

The design looks and works great using draft-quality layers at 0.3mm, but **you'll want to use PETG for the print**. While prototyping the case, I found PLA to be too brittle for the various posts that hold the components in place.

If your bridging is well tuned, you shouldn't need supports.

The knob/dial piece really should be printed with translucent PETG. This allows you to see the status light on the rotary encoder, and it's probably important for the IR signal to escape as well.

## Assembly

To use the included case design, wire up the components as described above. Sensors are held in place by slots, while posts hold the IR blaster, rotary encoder, and ESP32 board in place.

<img height=400 src="https://github.com/daniloc/ThermTerm/assets/213358/ef934ef7-89c7-4ac9-8dc7-f238c8ad32e6">

<img height=400 src="https://github.com/daniloc/ThermTerm/assets/213358/77c9fb7a-8b8f-41ef-94d9-ba864598fea4">


I affixed the small piezo buzzer to the metal RF shield on the ESP32 board to keep it out of the way.

<img height=400 src="https://github.com/daniloc/ThermTerm/assets/213358/4f9356d0-80bb-4e98-b148-467ddf758e92">

The rotary encoder breakout gets a lot of impact, so I'd suggest anchoring it in place by melting the bottom two posts with a soldering iron.

<img height=400 src="https://github.com/daniloc/ThermTerm/assets/213358/b8872f6b-8282-4a0a-8f0d-7ae7245192b0">

To manage the StemmaQT cables, use the raceway molded into the base to tuck the longest cable run, from ESP32 to the light sensor. For the shorter cables, fold them gently against the case to prevent the dial scraping against them.

<img height=400 src="https://github.com/daniloc/ThermTerm/assets/213358/06c2e1fd-6643-43c7-a856-0c1246508d2f">

If desired, print the key caps and press-fit them into the rectangular holes on the front of the case. Pop on the dial, and you're done.
