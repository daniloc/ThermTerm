# ThermTerm: an open source heat pump controller and home automation terminal, built on ESP32

**Control heatpumps manually or remotely, using MQTT/Home Assistant**.

I love my heat pumps. They're energy efficient and the fastest way to heat or cool any room.

But I've always hated the remote controls that come with heat pumps. They're clunky and hard to read, especially in low light. In theory, you can program schedules for your heat pumps, but in practice the remotes are too frustrating to use for that.

ThermTerm solves all the problems I've had with these physical controls, while integrating the heat pumps into Home Assistant via MQTT. Additional sensors pipe temperature, humidity and light data, for statistics and automation. Out of the box, ThermTerm supports Mitsubishi's infrared protocol, since that's the system I have. Thanks to integration with [IRRemoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266), you can make ThermTerm work with [your own heat pumps](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/SupportedProtocols.md) by writing a few lines of adaptor code. No signal analysis or reverse-engineering required. See [Customization](#customization) for more details.

## Interface

<p align="center">
<img width="527" alt="Diagram" src="https://github.com/daniloc/ThermTerm/assets/213358/17835a63-d495-4553-853f-562c07adade9">
</p>
Press the dial to toggle power.

Turn the dial to set temperature.

Change to heat mode with the bottom bottom, cool mode with the top. Set fan speed by pressing the center button, then turn the dial.

Through a TFT screen, readouts are visible any time of day. The chunky dial is easy to press without looking. Program automations using Home Assitant, not miserable rubber buttons on an awkward remote. Toggle your heat pumps from any room in the house, or even while you're away.

## Goals

- Make a heat pump controller I don't hate using
- Use easy-to-repurpose IR, instead of a manufacturer-specific serial connection
- Structure this project into discrete objects to make iteration, maintenance and enhancements easy
- Provide hobbyists with a reference for Arduino code with that kind of structure (I had a hard time finding it for end projects, only libraries)
- Build a device that expands the surface area of a home automation system, letting it gather more data, take more user input, and provide more feedback
- Leverage as much of the existing Arduino OSS code ecosystem as possible, keeping the project tightly scoped to its problem domain

## Bill of materials

- [Adafruit ESP32-S3 Feather Reverse TFT](https://www.adafruit.com/product/5691)
- [Adafruit I2C rotary encoder board with Neopixel](https://www.adafruit.com/product/4991)
- [Rotary encoder](https://www.adafruit.com/product/377)
- [IR blaster board](https://www.adafruit.com/product/5639)
- Optional: 5mm IR LED
- Optional: [Tiny piezo buzzer](https://www.adafruit.com/product/1740), for auditory feedback now (and later: alerts)
- Optional: [400 mAh LiPo battery](https://www.adafruit.com/product/3898), so the devices can still communicate with HA during a power outage
- [SHTC3 Temperature/humidity sensor](https://www.adafruit.com/product/4636)
- [BH1750 Light sensor](https://www.adafruit.com/product/4681)
- 1 [100mm StemmaQT/Qwiic cable](https://www.adafruit.com/product/4210)
- 2 [50mm StemmaQT/Qwicc cables](https://www.adafruit.com/product/4399)
- Wires for the IR blaster (I trimmed one of [these](https://www.adafruit.com/product/4336))
- USB-C cable ([right angle designs](https://www.amazon.com/dp/B08912ZBHZ) look best)
- A 5v USB power supply
- Opaque PETG to print the main case
- Translucent PETG to print the dial

The sensors use I2C, so with a light rewrite, you can replace the above models with alternatives if needed, or skip them entirely if you don't care about recording data back to Home Assistant.

If you want to design your own enclosure and use a different display, I suspect any ESP32-based device will work fine.

There's not a huge upside to the battery in the current implementation but I designed the case to accommodate it for future enhancements. You can skip it unless you want to tinker with power outage-related home automation tasks.

## Configuration

Make a copy of `_Credentials.h.example`, rename it to `_Credentials.h`. This file will be ignored by git.

Fill in these values as appropriate to your network:

```C++
#define WIFI_SSID ""       // your network SSID (name)
#define WIFI_PASSWORD "" // your network password

#define MQTT_LOGIN ""
#define MQTT_PASSWORD ""

#define MQTT_BROKER IPAddress(192, 168, 1, 100)
```

## Customization

The most urgent customization for most will be making this work with your own heat pump.

1. Write a subclass of `IRInterface`. You can use `MistubishiInterface` as a starting point. The goal is to translate the system controller's state model into whatever features you care about for your own heat pump. Consult IRremoteESP8266 for [the API that corresponds to your manufacturer](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/SupportedProtocols.md).
2. In `main.cpp`, replace the `MitsubishiInterface` instance in the `SystemController` constructor with your own interface:

```C++
    YourCustomInterface customInterface;
    SystemController controller(customInterface);
```

That should get you moving.

To tweak other details about the device, explore `_Constants.h`. To adapt this to alternative hardware, including other displays, you'll want to replace the implementations in `src/hardware`.

## Assembly

If you want to go all-in and asemble one of these for yourself, 3D printing files have been provided. See [ASSEMBLY.md](/enclosure/ASSEMBLY.md) for detailed instructions.

## Roadmap

There's a lot more this platform could do, but this is the minimally-useful implementation that can get your whole heat pump system integrated in with an afternoon of work. Future features:

- [ ] Display alerts issued from Home Assistant
- [ ] Trigger alerts distributed to all other ThermTerm devices on the network
- [ ] Set preferences, including WiFi and MQTT credentials, via serial terminal
- [ ] Bulk provisioning

## Contributions

If you have anything useful you'd like to give back, feel free. Of particular interest:

- I can only test with Mitsubishi 144-bit IR receivers, since that's all I have. If you have a working interface for a different manufacturer, open a PR to add it!
- This is the most C++ I've ever written. If there's anything goofy that could be done differently, feel free to propose it
- Relatedly, if you have thoughts on making this project more extensible with other displays and ESP32 variants, I'd love to hear about it
- Enhancements to documentation, or comments on code you found confusing, are great too
