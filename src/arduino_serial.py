import serial
import serial.tools.list_ports

ports = list(serial.tools.list_ports.comports())
for p in ports:
    print(p)
    if "Dispositivo seriale USB" in p.description:
        arduino_port = p.name

#arduino_port = "COM3" #serial port of Arduino
baud = 115200 #arduino uno runs at 9600 baud
fileName="analog-data.csv" #name of the CSV file generated

fileName = input("Name of file?")

if len(fileName) == 0:
    fileName = "data"

fileName = fileName+".csv"

ser = serial.Serial(arduino_port, baud)
print("Connected to Arduino port:" + arduino_port)
file = open(fileName, "a")
print("Created file")

samples = 10 #how many samples to collect

samples = input("How many samples? (sampling frequency of 100 Hz. 6000 samples = 1 min)")
samples = int(samples)

if samples == 0:
    samples = 6000 #1 minute
elif samples < 0:
    samples = 8640000 #24 hours

line = 0
ser.flushInput()

while line < samples:

    ser_bytes = ser.readline()


    #changes comma decimal separator format
    if len(ser_bytes) > 0:
        decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode("utf-8")
        data=decoded_bytes.replace(".",",")
        print(data)

        file.write(data + "\n") #write data with a newline
        line = line+1

print("Data collection complete!")
file.close()
