# Arduino-PIC-programmer
This projects enables programming PIC devices using Arduino only i.e. it should (in ideal cases) replace pickit and other programming devices. It currently only supports a very small number of devices which are PIC16F87XA family and uses Low voltage programming mode for obvious reasons. It should be able to work with some other devices as well but that is not guaranteed.

Support for more devices should be added in the next updates.

## Installation
### Prerequisites
All you need to start using this project is: 
- An Arduino uno (or any other Arduino but probably with a little modification to the pin numbers in the sketch).
- The [Arduino IDE](https://www.arduino.cc/en/Main/Software).
- And any python environment.

### Getting started
After you have installed the Arduino IDE and any python environment you need to install pyserial library in your python environment. It is usually done by typing this command in your console:
```
pip install pyserial
```
But it might not be that simple depending on your environment so if it didn't work just search how to install a library in your environment.

Next thing is you get the sketch file and the hextoserial.py file from this project, upload the sketch to the arduino using the IDE, connect the Arduino to the PIC as described in the beginning of the sketch.

## Usage
Having connected the arduino to the PIC as described in the sketch, and the arduino connected to your computer using USB, find out which port is your arduino connected to on your computer. This can be seen from the arduino IDE by opening port under Tools menu, You should find the port name (In windows, It is typically COMX where X is the port number).
