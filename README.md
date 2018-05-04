# Arduino-PIC-programmer
This projects enables programming PIC devices using Arduino only i.e. it should (in ideal cases) replace pickit and other programming devices. It currently only supports a very small number of devices which are PIC16F87XA family and uses Low voltage programming mode for obvious reasons. It should be able to work with some other devices as well but that is not guaranteed.
Also, this only supports HEX files of Intel HEX format (which is the most common format, I believe).

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
Having connected the arduino to the PIC as described in the sketch, and the arduino connected to your computer using USB, find out which port is your arduino connected to on your computer. This can be seen from the arduino IDE by opening port under Tools menu, You should find the port name (In windows, It is typically COMX where X is the port number), Remember that port name for the next step.
![](/screenshots/port.png)

Now run hextoserial.py in your environment. You will be asked for the name of the port where the arduino is connected. After entering it you will be asked to enter the full path of the hex file. Then it asks you to enter P for programming, V for verification or D for verification with debugging, You should start by Entering P then after the programming is done enter V.

If the programming was done correctly the verification process shouldn't produce any error messages and end with "Verification done", otherwise you will get an error message saying verification failed. You may then try reprogramming by entering P or verify with debugging using D to show you the difference between the file and the data it reads from the chip.

After you are done stop the process in the console, this should close the serial communication with the arduino and close the hex file.

## Example
Here is a circuit diagram of the connection:
![](/screenshots/schematic.PNG)

Make sure you put a 10K resistor on the MCLR pin and a 100nF capacitor to the ground as shown above.

Connecting the arduino to the computer and uploading the sketch, then running the python file:
![](/screenshots/console1.PNG)

Entering the communication port and the path to the hex file:
![](/screenshots/console2.PNG)

Choosing P to program then V to verify programming:
![](/screenshots/console3.PNG)

And that's it. Now connect the pic in its normal connection or if it is on a board remove the connections to the arduino and restart.
