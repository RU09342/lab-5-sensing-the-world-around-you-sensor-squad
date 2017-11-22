% Code by: Ryan Hare
% Last modified on: 11/22/17
% To read and plot data outputs from an MSP430 microcontroller.

%% Create an object for the serial port
serialPort = 'COM12';
inputPort = serial(serialPort);
fopen(inputPort);

%% Set the plotting window
time = now;
serialData = 0;

dataWindow = figure('NumberTitle','off',...
    'Name','Serial Data',...
    'Color',[0 0 0],'Visible','off');

% Set up axes
dataAxes = axes('Parent',dataWindow,...
    'XGrid','on',...
    'XColor',[0.75 0.75 0.75],...
    'YGrid','on',...
    'YColor',[0.75 0.75 0.75],...
    'Color',[0 0 0]);

hold on;

plotData = plot(dataAxes,time,serialData,'Marker','.','LineWidth',1,'Color',[0 1 0]);

xlim(dataAxes,[min(time) max(time+0.001)]);

% Label x axis
xlabel('Time','FontWeight','bold','FontSize',14,'Color',[1 1 0]);

% Label y axis
ylabel('Output value','FontWeight','bold','FontSize',14,'Color',[1 1 0]);

% Label graph
title('Serial Data','FontSize',15,'Color',[1 1 0]);

%% Set the time span and interval for data collection
stopTime = rem(now,1) + 0.0007; %This should be 60 seconds after current time
timeInterval = 0.005;

%% Collect data
count = 1;

while rem(now,1) < stopTime
    time(count) = datenum(clock); 
    serialData(count) = fscanf(inputPort,'%f');
    set(plotData,'YData',serialData,'XData',time);
    set(dataWindow,'Visible','on');
    datetick('x','mm/DD HH:MM');
    
    pause(timeInterval);
    count = count +1;
end

%% Close the serial object after collecting data
fclose(inputPort);
delete(inputPort);
clear inputPort;