# Nixie-Tube-Clock
This project is based on the Easy Nixie modules made by allexoK. The following link is the hackaday page for it: https://hackaday.io/project/192446-easynixie
The circuit itself is decently simple, the version that is built in the photos doesn't include the capacitors but has issues with some power supplies as the striking wattage (turn on wattage) of the lights is quite above the specced 10 watt power supply. The chip used for the logic is just a generic esp32. As a fair warning, the side usb port was put there as a way to program the esp32 without having to take in out its header pins too many times, however, if the switch isn't flipped to the correct side, no power will be supplied.
Some improvements could be made the the electrical design. If you don't prefer having an both LED's be addressible, the posts that hold up the LED's could be used as the conductor the the second plane. 

![Pictures (1 of 1)](https://github.com/user-attachments/assets/e4787406-f105-4748-95b8-8e9265b6fd7e)
