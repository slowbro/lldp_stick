# LLDP Stick PCB

# How to Order (JLCPCB)

1. Zip all `Gerber_` and `Drill_` files into one zipfile.
2. Upload to JLCPCB.
3. Selection the following options (use defaults if not specified):
    - Confirm Production File: yes
    - Remover Order Number: Specify a Location
4. Optional: Select PCB Assemby, using the following options:
    - PCBA Type: Economic
    - Assemby Side: Top
    - Tooling Holes: Added by Customer
    - Confirm Parts Placement: Yes
5. Upload 'BOM_LLDPStick-v3.x.csv' as BOM.
6. Upload 'PickAndPlace_PCB_LLDPStick-v3.x.csv' as CPL.
7. Confirm parts on next page.
    - You may need to replace capacitors/resistors as stock levels tend to shift; make sure you use the same size/capacity!
    - Ignore/"Do Not Place" for the OLED screen.
8. Place your order.
