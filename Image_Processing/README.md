# Image Processing

Absolute or relative  
1. absolute value (abs)  
compares individual pixel against threshold (usually 256/2) 
  
2. relative value (rel)  
compares individual pixel against mean of surrounding 3~8 pixels  
  
Greyscale or blackwhite  
1. greyscale  
sets processed rgb value to be equal to mean of original rgb value  
2. blackwhite  
sets rgb value of pixel to (0, 0, 0) or (255, 255, 255)  
  
# Conversion to 3D-coordinates

The script takes note of the positions where the pixel switches from black to white or vice versa when traversing across the pixels from bottom left to top right.  
The coordinates in the text file indicate when the servo motor of the drawing machine lifts or drops the pen.  
  
Format: 'x y z'  
[Example instruction.txt](https://github.com/Andrew-Gan/frm-45/blob/master/Image_Processing/instruction.txt)    
x - horizontal position scaled to size of paper (mm)  
y - vertical position scaled to size of paper (mm)  
z - pen position: 0 for pen up, 1 for pen down  
  
Note: if pen pressure may be controlled, values between 0 and 1 may be used for the z value to indicate darkness of stroke.  
  
Needed:  
A script running on the STM32 microcontroller than relays the information from the instruction.txt to the stepper and servo motors in the Eleksmaker drawing machine.  