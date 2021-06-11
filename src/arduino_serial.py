import serial
import tkinter as tk
from tkinter import filedialog

arduino_port = "COM3" #serial port of Arduino
baud = 115200 #arduino uno runs at 9600 baud
fileName="analog-data.csv" #name of the CSV file generated



root = tk.Tk()
root.withdraw()

fileName = filedialog.asksaveasfilename()

ser = serial.Serial(arduino_port, baud)
print("Connected to Arduino port:" + arduino_port)
file = open(fileName, "a")
print("Created file")

samples = 10 #how many samples to collect

samples = input("Enter your value: ")
samples = int(samples)

print_labels = False
line = 0 #start at 0 because our header is 0 (not real data)
while line <= samples:
    # incoming = ser.read(9999)
    # if len(incoming) > 0:
    if print_labels:
        if line==0:
            print("Printing Column Headers")
        else:
            print("Line " + str(line) + ": writing...")
    getData=str(ser.readline())

    #changes comma decimal separator format

    #data=getData[0:][:-2]
    data=getData[3:][:-5].replace(".",",")
    print(data)

    file = open(fileName, "a")
    file.write(data + "\n") #write data with a newline
    line = line+1

print("Data collection complete!")
file.close()
