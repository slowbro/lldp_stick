# v3 PCB Changelog

## Rev 6
- Switch to cheaper HanRun RJ45 w/ integrated magnetics
- Update battery monitoring voltage divider to add filter/stability capacitor and take advantage of more of the ADC range (thanks Adaruit)

## Rev 5
- Much like IPv5, this doesn't really exist

## Rev 4
- Switch to LDO + cheaper battery management chip for cost and energy savings (thanks Adafruit for the design borrowed from nRF52 Feathers)
- Re-add inductor for nRF52832 DC-DC Converter
- Remove SCL/SDA test points

## Rev 3
- Remove superfluous inductor for nRF52832 DC-DC Converter (since the on-board DC-DC prevents us from dipping below 3.3V)

## Rev 2
- Add `PERIPH` transistor to switch OLED, WizNet on/off
- Remove erroneous resistors on buttons

## Rev 1
- Fix spacing on pogo-pin programming header
- Add test points for SCL/SDA

## Rev 0
- Initial board
