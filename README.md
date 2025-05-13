# Nixie-Tube-Clock
This project is based on the Easy Nixie modules made by allexoK. The following link is the Hackaday page for it: https://hackaday.io/project/192446-easynixie

The circuit itself is decently simple, the version built in the photos doesn't include the capacitors but has issues with some power supplies as the lights' striking wattage (turn on wattage) is quite above the specified 10-watt power supply. The chip used for the logic is just a generic esp32. A wire connects P27 to a plate of brass on the top of the clock. As you touch the plate it will turn on for 5 seconds (specified in code).

As a fair warning, the side usb port was put there to program the esp32 without having to take it in and out of its header pins too many times; however, if the switch isn't flipped to the correct side, no power will be supplied. An alternative is to set up the esp32 to be writable OTA.

Some improvements could be made to the electrical design. If you don't prefer having an both LED's be addressible, the posts that hold up the LED's could be used as the conductor to the second plane. 

![Pictures (1 of 1)](https://github.com/user-attachments/assets/e4787406-f105-4748-95b8-8e9265b6fd7e)
