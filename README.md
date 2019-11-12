## Curent STM Size
```
   text	   data	    bss	    dec	    hex	filename
  47428	   2492	    752	  50672	   c5f0	SD_Card_std.elf
    332	      0	      0	    332	    14c	src/button.o
  24454	      0	    266	  24720	   6090	src/ff.o
      0	      0	      0	      0	      0	src/ffsystem.o
   1656	      0	      0	   1656	    678	src/ffunicode.o
   1149	      0	      3	   1152	    480	src/main.o
    196	      0	      0	    196	     c4	src/my_spi.o
    824	      0	      8	    832	    340	src/oled.o
   1064	      0	     12	   1076	    434	src/parser.o
   3376	      1	      1	   3378	    d32	src/sd_card.o
    594	      4	      8	    606	    25e	src/syscalls.o
    512	     20	      0	    532	    214	src/system_stm32f0xx.o
    468	      0	      0	    468	    1d4	src/timer.o
```

**Still Overflow by 924 bytes**


# FRM-45
ECE362 Project 1 Drawing Machine  

Stepper and Servo Motors  
[SD Card Programming](https://github.com/Andrew-Gan/frm-45/tree/master/SD_Card_std)  
[Parser for Limited G-Code](https://github.com/Andrew-Gan/frm-45/tree/master/Parser)  

## Used Pins
* **PA8** - _servomotor_  
* **PA4-7** - _sd card reader_
* **PB12-15**  - _OLED display_
* **PA1-PA2**  - _EXTI interrupt_
  
## SD Card and OLED References
---
- [Reading From SD Card](http://shukra.cedt.iisc.ernet.in/edwiki/Reading_and_writing_to_a_file_on_sd_card_using_stm32_discovery_kit)
- [Example from book](https://docs.google.com/file/d/0B8buOhyZhvwZRU9XWHBpYUYyUnc/edit)
- [Open Source Library](http://elm-chan.org/fsw/ff/00index_e.html)
- [I/O Pin Setup](https://sites.google.com/site/embeddedfun01/fat-filesystem-on-sd-card)
- [SPI and SD Cards reading](http://www.dejazzer.com/ee379/lecture_notes/lec12_sd_card.pdf)
- [FRM 745](https://engineering.purdue.edu/ece362/refs/STM32F0x1_Family_Reference.pdf#%5B%7B%22num%22%3A6629%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22XYZ%22%7D%2C67%2C226%2Cnull%5D)
- [Standard Peripheral Library Manual](https://www.st.com/content/ccc/resource/technical/document/user_manual/59/2d/ab/ad/f8/29/49/d6/DM00023896.pdf/files/DM00023896.pdf/jcr:content/translations/en.DM00023896.pdf)
- [Another example of STM32 of SD card](https://stm32f4-discovery.net/2014/07/library-21-read-sd-card-fatfs-stm32f4xx-devices/)
- [SPI presentation from STM](https://www.st.com/content/ccc/resource/training/technical/product_training/group0/3e/ee/cd/b7/84/4b/45/ee/STM32F7_Peripheral_SPI/files/STM32F7_Peripheral_SPI.pdf/_jcr_content/translations/en.STM32F7_Peripheral_SPI.pdf)

![SD_init](Sdinit.jpg)
  
## GCode References
* [Data Sheet for A4988](https://www.digchip.com/datasheets/parts/datasheet/029/A4988-pdf.php)
* [CoreXY CNC Plotter](https://www.instructables.com/id/CoreXY-CNC-Plotter/?fbclid=IwAR0pwp5_IWx3ZKXX_-JnrqQA2Jc5w_0K_mVkKk8bpvbu7aeOSSTUAUPy2AY) 
* [How to read GCode](https://makezine.com/2016/10/24/get-to-know-your-cnc-how-to-read-g-code/?fbclid=IwAR3-w1awEFc9UalxPZxf2Twn3zoppbnQXDNl3gHNShbNhc7IxlubZtNi5JA)
* [Gcode Reference](https://www.cnccookbook.com/g-code-m-code-reference-list-cnc-mills/)
* [GCode G02 G03](https://www.cnccookbook.com/cnc-g-code-arc-circle-g02-g03/)  
* [Servomotor with STM32](https://controllerstech.com/servo-motor-with-stm32/)  
* [Another website about how to make gcode](https://www.marginallyclever.com/2013/08/how-to-build-an-2-axis-arduino-cnc-gcode-interpreter/?fbclid=IwAR2Cuwl6wh2F5Wqw8JK4fef5dCa7CKCwGwoWh0Y8hn-iv6d_DvxPZTSHs24)
* [Bresenham's line algorithm to draw a line between 2 points](https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm)

![G-Code](Parser/g-code.png)
