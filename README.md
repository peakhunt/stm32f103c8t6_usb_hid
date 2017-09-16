# stm32f103c8t6_usb_hid

This is a USB joystick demo using STM32F103C8T6.
The whole project was developed using CubeMX.

## hardware
Basically I used very cheap chinese STM32F103 module. [The link is here][1]

As for arcade style joysticks and push buttons, there are a bunch of
cheap chinese stuffs out there.

As for connection, please refer to the kicad directory. It's gonna cost about $10 US dollars to get the 10 custom PCBs from cheap PCB manufactureres. Whether you use custom soldering or PCB based approach, just be sure to pay attention to the capacitor to prevent debouncing .  And just bear in mind that I am a S/W enginner. Designing a simple H/W like this is just a kind of my hobby.

[1]: https://www.aliexpress.com/item/Free-Shipping-STM32F103C8T6-ARM-STM32-Minimum-System-Development-Board-Module-For-arduino/32555258029.html?spm=2114.search0104.3.89.mrWsfD&ws_ab_test=searchweb0_0,searchweb201602_5_5460015_10152_10065_10151_10068_5470015_10307_10137_10060_10155_10154_10056_10055_10054_10059_100031_10099_10338_10339_5380015_10103_10102_440_10052_10053_10142_10107_10050_10051_10326_10084_10083_10080_10082_10081_10110_10175_10111_10112_10113_10114_143_10037_10312_10313_10314_10078_10079_10077_10073,searchweb201603_19,ppcSwitch_5&btsid=67a13b36-0190-4c70-9deb-1a2744050228&algo_expid=c62760bd-a140-4c3e-86ec-c725bdf3f0dc-14&algo_pvid=c62760bd-a140-4c3e-86ec-c725bdf3f0dc

## my build
Here is a demo joystick I built. I've been having some fun using it with retroarch. I love kungfu master.

![joystick build](https://github.com/peakhunt/stm32f103c8t6_usb_hid/blob/master/imgs/IMG_20170916_200444.jpg)

![joystick build inside](https://github.com/peakhunt/stm32f103c8t6_usb_hid/blob/master/imgs/IMG_20170916_200453.jpg)


The case is actually a small rectangular plastic container ( or basket or whatever). I bought it as less than 2 dollars at a local store and drilled it.

Here is the image of PCB I created for the build. It's really primitive simple board and anybody can do it. I just hated all the stupid fragile wiring I had to do before the PCB.

![jostick board](https://github.com/peakhunt/stm32f103c8t6_usb_hid/blob/master/imgs/IMG_20170916_200824.jpg)
