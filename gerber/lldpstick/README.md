# LLDP Stick PCB

# How to Order (JLCPCB)

1. Zip all `Gerber_` and `Drill_` files into one zipfile.
2. Upload zipfile to JLCPCB: https://jlcpcb.com/
3. Selection the following options (use defaults if not specified):
    - Surface Finish: LeadFree HASL
    - Confirm Production File: yes
    - Remove Order Number: Specify a Location
4. Select PCB Assemby, using the following options:
    - PCBA Type: Economic
    - Assemby Side: Top
    - Tooling Holes: Added by Customer
    - Confirm Parts Placement: Yes
5. Upload 'BOM_LLDPStick-v3.x.csv' as BOM.
6. Upload 'PickAndPlace_PCB_LLDPStick-v3.x.csv' as CPL.
7. Confirm parts on next page.
    - You may need to replace capacitors/resistors as stock levels tend to shift; make sure you use the same size/capacity!
    - You may need to manually search for and select the RJ45 connector (RJ1) in order to have it placed/assembled.
    - Ignore/"Do Not Place" for the OLED screen.
8. Place your order.
