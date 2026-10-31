# ESP32 S3 WiFi TCPIP Keyboard

Connect a USB Keyboard to the USB (not the COM) port of the ESP32 S3 and receive the typed text over TCPIP.
Note: The USB port should be USB-OTG enabled.

## Configure

Create an additional file *.platform.ini*

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
