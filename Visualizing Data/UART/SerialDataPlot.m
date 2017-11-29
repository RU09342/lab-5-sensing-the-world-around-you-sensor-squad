% Code by: Ryan Hare
% Last modified on: 11/22/17
% To read and plot data outputs from an MSP430 microcontroller.

%% Create an object for the serial port
serialPort = 'COM9';
inputPort = serial(serialPort,'BaudRate',9600);
fopen(inputPort);

%% Collect and plot data
x=linspace(1,100);

for i=1:length(x)
    fprintf("01");
    y(i)=fscanf(inputPort); 
end

plot(x,y)

%% Close the serial object after collecting data
fclose(inputPort);
delete(inputPort);
clear inputPort;