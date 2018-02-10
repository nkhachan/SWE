import serial #Import the pyserial module
import matplotlib.pyplot as plt  #Import the matplotlib module
import scipy.signal
count = 0
datafile = raw_input("Please enter the file that you want to store the data in") #Prompt user for input file
f = open(datafile, 'w') # Open file to write in
x_data=[] #Create empty arrays
y_data=[]
y_scaled_data=[]
data = serial.Serial('COM5', 115200) #Create a serial connection with arduino
while count < 1000:  #Only collect 1000 samples, or 5 seconds worth of data
	info = data.readline()
	myList = info.split('\t') #Make an array out of a line
	f.write(myList[0])  #Write time to file
	f.write('\t')
	f.write(myList[1])  #Write raw EMG value to file
	f.write('\n')
	x_data.append(float(myList[0])/1000000)  #Add the time values to x_axis list
	y_data.append(float(myList[1]))  #Add the raw EMG values to y_axis list
	y_scaled_data.append((((float(myList[1]))*(3.3/1024))-1.5)/3.6)  #Create scaled array of values
	count = count + 1

b_high, a_high = scipy.signal.butter(3,0.1,'high',analog=False) #Find coefficients for high-pass
b_low, a_low = scipy.signal.butter(3,0.5,'low',analog=False) #Find coefficients for low-pass
y_high = scipy.signal.lfilter(b_high,a_high,y_scaled_data) #Apply high-pass filter
y_both = scipy.signal.lfilter(b_low,a_low,y_high) #Apply high-pass filter
first = plt.subplot(3,1,1) #Make two subplots, and start writing in first one
first.set_title("EMG Voltage with High/Low Pass Filters")
plt.plot(x_data,y_both)  #plot the raw values x_data vs y_data
plt.ylabel("EMG Voltage (mV)")  #Label Axes
plt.xlabel("Time(seconds)")

second = plt.subplot(3,1,2)  #Plot in second subplot
second.set_title("Rectified Signal")
y_rect= abs(y_both)
plt.plot(x_data,y_rect)  #plot the rectified values
plt.ylabel("EMG Voltage (mV)")  #Label Axes
plt.xlabel("Time(seconds)")

third = plt.subplot(3,1,3)  #Plot in third subplot
third.set_title("Smoothed Signal")
box = scipy.signal.boxcar(100)
y_box = scipy.signal.lfilter(box,1,y_rect)
plt.plot(x_data,y_box)  #plot the smoothed values
plt.ylabel("EMG Voltage (mV)")  #Label Axes
plt.xlabel("Time(seconds)")

plt.show()
