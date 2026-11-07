# ESP32 S3 WiFi TCPIP Keyboard

Connect a USB Keyboard to the USB (not the COM) port of the ESP32 S3 and receive the typed text over TCPIP.
Note: The USB port should be USB-OTG enabled.

## Build

1. Install PlatformIO in VSCode
2. git clone https://github.com/lihardyextreme/esp32-s3-host-usb-keyboard-to-wifi-tcpip.git
3. Open folder of the cloned repository in VSCode
4. Press Alt+Ctrl+B

## Configure

Create an additional file *.platformio.ini*

```
[wifi]
ssid="your-ssid"
password="your-password"
```

## Usage

1. Connect a keyboard to the ESP32-S3's USB port with enabled USB-OTG
2. On your PC: connect to server `telnet esp32-keyboard.local 9009'
3. Type on the keyboard (1) and receive the text in the telnet terminal (2)
4. In order to disconnect type a dot '.' in the telnet terminal (2)

## Special key combinations

ESP32 Keyboard (1):
- Magic combo: (Left-Ctrl) + (Left-Alt) + (Left-Windows-key)
- Reboot ESP32: (Magic combo) + (Break/Pause)
- Disconnect ESP32 tcpip client: (Magic combo) + Esc (Escape)

### Keyboard layout

In order to change the keyboard layout, on the ESP32 keyboard (1) use the following sequences:

- english (default): (Magic combo) + e
- german (deutsch): (Magic combo) + d
