# USBC-DAM Programmer

Use this programmer to expose SWD for programming the LLDPStick. You will need to use a Thunderbolt 3+ cable on the target side - or at the very least, a cable that has all pins connected.

## How to Order (JLCPCB)

1. Zip all `Gerber_` and `Drill_` files into one zipfile.
2. Upload zipfile to JLCPCB: https://jlcpcb.com/
3. Selection the following options (use defaults if not specified):
    - Confirm Production File: yes
4. Select PCB Assemby, using the following options:
    - PCBA Type: Economic
    - Assemby Side: Top
    - Tooling Holes: Added by JLCPCB
    - Confirm Parts Placement: Yes
5. Upload 'BOM_*.csv' as BOM.
6. Upload 'PickAndPlace_*.csv' as CPL.
7. Confirm parts on next page.
    - You may need to replace capacitors/resistors as stock levels tend to shift; make sure you use the same size/capacity!
8. Place your order.
