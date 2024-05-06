# LLDP Stick

![PCB Image](pcb.png)

## What is it?
A network diagnostic tool. Initial goal is to show LLDP information; future goal is to allow more network diagnostics over BLE with a phone app.

## Goals

- [ ] Additional diagnostics via BLE
    - [ ] Set/reset MAC Address
    - [ ] DHCP
    - [ ] Ping
- [ ] Support CDP
- [x] Reduce idle power draw (and thus improve battery life) - success! 34->7mA at idle
    - [ ] Reduce idle power draw MOAR
- [ ] 1G PHY: Support switches that don't go down to 100M (i.e. Arista DCS-7280TR)
- [ ] Cost reduction: $20-$30 per stick

## Dependencies

Requires `s132_nrf52_2.0.1_softdevice.hex` to be placed in `~/.platformio/packages/framework-arduinonordicnrf5/cores/nRF5/SDK/components/softdevice/s132/hex/`. The binary can be downloaded from Nordic Semi [here](https://www.nordicsemi.com/Products/Development-software/nRF5-SDK/Download) under '11.0.0nRF5 SDK'.

## Building

1. Install [PlatformIO](https://platformio.org/).
2. Clone the repository.
3. See "Dependencies" above - download and copy the SoftDevice hex file to the proper location. You may need to try `pio run` first to download the apporiate libraries.
4. Run platformio: `pio run`.
5. To upload: plug in your J-Link (or other programmer) and run `pio run -t upload`.

## Assembly

You will need:
- The PCB; you can order this from JLCPCB assembled using the included gerber/bom/placement files.
- An 128x64 i2c OLED Display with a SSD1306 controller and FPC connector; I buy from [BuyDisplay](https://www.buydisplay.com/datasheet-128x64-oled-module-spi-0-96-inch-graphic-displays-white-on-black).
- 2x Buttons for the top side: [KSC441JST1LFS at Mouser](https://www.mouser.com/ProductDetail/CK/KSC441JST1LFS?qs=8LLMQjkBYsII%252BtWfhZrIfA%3D%3D) - not available for assembly from JLC.
- Dual-LED chip [LTST-C195KGJRKT at Mouser](https://mou.sr/4auO7dj)
- A small LiPo battery such as DU LITER [902035](https://bihuade.com/products/902035-37v-600mah-liter-energy-battery-polymer-lithium-rechargeable-battery-for-mp3-gps-dvd-bluetooth-recorder-e-book-camera) (600mAh), or DU LITER [802245](https://bihuade.com/products/37v-802245-1000mah-bihuade-lithium-polymer-rechargeable-battery-for-bluetooth-headset-speaker) (1000mAh).
- The 3D printed case.
- 2x M4x2mm screws.

Steps:
1. [Order your PCBs from JLCPCB](gerber/README.md) (or your manufacturer of choice - the part selection is such that assembly from JLC is turnkey). Continue once PCBs have arrived.
2. Solder the OLED FPC connector.
3. Solder buttons onto the front (back?) of the PCB. Also solder the dual LED package if you opted to only have one side of the PCB assembled.
4. Fold the OLED connector over, and superglue the OLED so that the top of the OLED panel is about flush with the top of the PCB.
5. Flush-trim the RJ-45 connector pins.
6. Program the board with your favorite SWD programmer. I use a J-LINK.
7. 3D print the case - body and one of the lids. Use the development lid if you plan on developing the board, as it will let you hang a SWD connector out the back.
8. Angle the RJ45 side of the board into the case, and then push down on the USB-C end to fit the PCB into the case. Secure with 1-2 M2x4mm screws.
9. Plug in the battery and (ideally) hotglue it in place in the case.
10. Slide the "lid" printed part into its slot on the back of the body of the case.

## Approximate Cost
...assuming you're assembling 5 at a time:

- PCB: $33.75 assembled (assuming top-side-only assembly, DHL shipping, JLCPCB, QTY 5)
- Screen: $3.56
- Battery: $6.40
- Misc components (LED, buttons, screws, case): ~$4

Total: $47.71/ea
