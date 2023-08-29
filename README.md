# ThermTerm: an open source heat pump controller and home automation terminal, built on ESP32
I love my heat pumps. They're energy efficient and the fastest way to heat or cool any room.

But I've always hated the remote controls that come with heat pumps. They're clunky and hard to read, especially in low light. In theory, you can program schedules for your heat pumps, but in practice the remotes are too frustrating to use for that.

ThermTerm solves all the problems I've had with these physical controls, while integrating the heat pumps into Home Assistant via MQTT. Additional sensors pipe temperature, humidity and light data, for statistics and automation. Out of the box, ThermTerm supports Mitsubishi's infrared protocol, since that's the system I have. Thanks to integration with [IRRemoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266), you can make ThermTerm work with [your own heat pumps](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/SupportedProtocols.md) by writing a few lines of adaptor code. No signal analysis or reverse-engineering required. See [Customization](#customization) for more details.

## Interface

Press the dial to toggle power.

Turn the dial to set temperature.

Change modes or set fan speed using the buttons on the left.

Through a TFT screen, readouts are visible any time of day. The chunky dial is easy to use without looking at all. Program automations using Home Assitant, not miserable rubber buttons on an awkward remote. Toggle your heat pumps from any room in the house, or even while you're away.

## Goals

- Make a heat pump interface I don't hate using
- Use a flexible, broadly-useful interface, IR, instead of a manufacturer-specific serial connection
- Structure this project into discrete objects to make iteration, maintenance and enhancements easy
- Provide hobbyists with a reference for Arduino code with that kind of structure (I had a hard time finding it for end projects, only libraries)
- Build a device that expands the surface area of a home automation system, letting it gather more data and provide more feedback
- Leverage as much of the existing Arduino code ecosystem as possible, keeping the project tightly scoped to its problem domain

## Bill of materials

- Adafruit ESP32-S3 Reverse TFT
- Adafruit I2C rotary encoder board with Neopixel
- IR blaster board
- Optional: 5mm IR LED
- Optional: Piezo buzzer, for auditory feedback now and later: alerts
- Optional: LiPo battery, so the devices can still communicate during a power outage
- Temperature/humidity sensor
- Light sensor
- 3 100mm StemmaQT/Qwiic cables
- 1 50mm StemmaQT/Qwicc cable
- Opaque PETG to print the main case
- Translucent PETG to print the dial

The sensors use I2C, so with a light rewrite, you can replace the above models with alternatives if needed, or skip them entirely if you don't care about recording data.

If you want to design your own enclosure, I suspect any ESP32-based device will work fine.

I do a lot with backup power, but if you don't, I'm sure you can skip the battery without trouble.

## Customization

The most urgent customization for most will be making this work with your own heat pump.

1. Write a subclass of IRInterface. You can use MistubishiInterface as a starting point. The goal is to translate the system controller's state model into whatever features you care about for your own heat pump.
2. In `main.cpp`, replace MitsubishiInterface in the `SystemController` constructor with your own interface:

```C++
    YourCustomInterface customInterface; // Create alternative IR schemes by sublassing IRInterface
    SystemController controller(customInterface);
```

That should get you moving.

To tweak other details about the device, explore `_Constants.h`. To adapt this to alternative hardware, including other displays, check out `src/hardware`.

## Assembly

If you want to go all-in and asemble one of these for yourself, 3D printing files have been provided. See ASSEMBLY.md for detailed instructions.

## Roadmap

There's a lot more this platform could do, but this is the minimally-useful implementation that can get your whole heat pump system integrated in with an afternoon of work. Future features:

- [] Display alerts issued from Home Assistant
- [] Trigger alerts distributed to all other ThermTerm devices on the network
- [] Bulk provisioning

## Contributions

If you have anything useful you'd like to give back, feel free. Of particular interest:

- I can only test with Mitsubishi 144-bit IR receivers, since that's all I have. If you have a working interface for a different manufacturer, open a PR!
- This is the most C++ I've ever written. If there's anything goofy that could be done differently, feel free to propose it
- Relatedly, if you have thoughts on making this project more composable with various hardware, all for it
- Enhancements to documentation, or comments on code you found confusing, are great