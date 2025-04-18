# AWTRIX STREAM

A project that displays information from an AWTRIX matrix on an LC display via HTTP communication.

## Description

This project interfaces with an AWTRIX matrix display through HTTP requests and presents the information on an LC display.

For more detailed information about this project, please visit:
- Project blog: [datort.de](https://datort.de)
- Video tutorial: [YouTube Tutorial](https://youtu.be/LMmw3sYUiGc) - A detailed walkthrough of the project and build instructions in german language
- The AWTRIX main project: [blueforcer.github.io](https://blueforcer.github.io/awtrix3/#/)


## Requirements

- ESP8266 D1 Mini board (others may work, not tested yet)
- LC display with ST7789 driver support
- A host AWTRIX matrix to grab the display from

## Building and Setup

1. Clone this repository
2. Open the project in PlatformIO
3. Configure your settings in `platformio.ini`
4. Build and upload to your device

## MQTT Support

When MQTT is configured in the initial setup, you can control your AWTRIX STREAM device through MQTT messages. 

You can configure the topic AWTRIX STREAM listens on during your initial configuration. For the following examples we will use the default of "awtrixstream".

- `awtrixstream/erase` - Caution: Will immediately erase all settings to start over
  - Payload: none

- `awtrixstream/reboot` - Restart your AWTRIX STREAM
  - Payload: none

- `awtrixstream/setting` - Modify device settings
  - Payload examples:
    ```json
    {"awtrixHostname": "192.168.2.3"}  // Changes the ip/host of the host AWTRIX to grab data from
    {"awtrixAuthUser": "myuser"}  // Changes username for HTTP authentication to the host AWTRIX
    {"awtrixAuthPass": "mypass"}  // Changes password for HTTP authentication to the host AWTRIX
    {"mqttBroker": "my.broker.local"}  // Changes the broker url
    {"mqttTopic": "awtrixstream123"}  // Changes the topic the AWTRIX STREAMS listens on for these type of commands
    ```

- `awtrixstream/display` - Toggle the display on/off
  - Payload examples:
    ```json
    {"on": true}  // Activate the display
    {"on": false}  // Deactivate the display
    ```


## License

This project is licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) License.

This license allows you to:
- Share: copy and redistribute the material in any medium or format
- Adapt: remix, transform, and build upon the material

Under the following terms:
- Attribution: You must give appropriate credit, provide a link to the license, and indicate if changes were made.
- NonCommercial: You may not use the material for commercial purposes.
- ShareAlike: If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.

For more information about the license, visit: [Creative Commons License](https://creativecommons.org/licenses/by-nc-sa/4.0/) 