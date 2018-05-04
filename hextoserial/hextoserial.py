try:
    import serial
except:
    print("Couldn't import serial module. Try installing pyserial library first.")
    exit(1)

port = input("Enter the name of the port: ")
try:
    ser = serial.Serial(port.strip(" ").capitalize(), 9600)
except:
    print("Couldn't open serial communication check the port name.")
    exit(1)

filename = input("Enter the full filename (full path): ")
try:
    hexFile = open(filename, "r")
except:
    print("Couldn't open the hex file. Make sure to enter the full path to the file.")
    ser.close()
    exit(1)

hexDict = {}
for line in hexFile:
    # skipping configuration word and end of file line
    if int(line[3:7], 16) // 2 != 0x2007 and len(line) != 11:
        address = int(line[3:7], 16) // 2
        n = int(line[1:3], 16) // 2
        for i in range(n):
            # storing the word in the form of a tuple of two bytes
            hexDict[address] = (bytes.fromhex(line[9 + 4 * i:11 + 4 * i]), bytes.fromhex(line[11 + 4 * i:13 + 4 * i]))
            address += 1

# filling empty addresses in the middle with un-programmed word 0x3FFF
for i in range(max(hexDict.keys()) + 1):
    if i not in hexDict.keys():
        hexDict[i] = (b'\xff', b'\x3f')
def main():
    byte = b''
    i = 0
    while True:
        if ser.inWaiting() > 0:
            byte = ser.read()
            if byte == b'\x01':
                # Send new word
                ser.write(hexDict[i][0])
                ser.write(hexDict[i][1])
                i += 1
            elif byte == b'\x02':
                # Send user input
                ser.write(input().encode("utf-8"))
            elif byte == b'\x03':
                # return to the first word
                i = 0
            elif byte == b'\x04':
                # Send the number of words in 2 bytes
                ser.write(bytes([len(hexDict.keys()) & 0b11111111, len(hexDict.keys()) >> 8]))
            else:
                # print received byte(character)
                print(byte.decode("ascii"), end="")


if __name__ == '__main__':
    try:
        main()
    except:
        ser.close()
        hexFile.close()
