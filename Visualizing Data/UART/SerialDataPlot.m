% Code by: Ryan Hare
% Last modified on: 11/21/17
% To read and plot data outputs from an MSP430 microcontroller.

%% Create an object for the serial port
serialPort = 'COM3';
serialObject = serial(serialPort);
fopen(serialObject);

%% Set the instrument in remote mode
fprintf(serialObject,'SYSTEM:REMOTE');

%% Set up the figure window
time = now;
voltage = 0;

figureHandle = figure('NumberTitle','off',...
    'Name','Voltage Characteristics',...
    'Color',[0 0 0],'Visible','off');

% Set axes
axesHandle = axes('Parent',figureHandle,...
    'YGrid','on',...
    'YColor',[0.9725 0.9725 0.9725],...
    'XGrid','on',...
    'XColor',[0.9725 0.9725 0.9725],...
    'Color',[0 0 0]);

hold on;

plotHandle = plot(axesHandle,time,voltage,'Marker','.','LineWidth',1,'Color',[0 1 0]);

xlim(axesHandle,[min(time) max(time+0.001)]);

% Create xlabel
xlabel('Time','FontWeight','bold','FontSize',14,'Color',[1 1 0]);

% Create ylabel
ylabel('Voltage in V','FontWeight','bold','FontSize',14,'Color',[1 1 0]);

% Create title
title('Voltage Characteristics','FontSize',15,'Color',[1 1 0]);

%% Set the time span and interval for data collection
stopTime = '10/07 21:53';
timeInterval = 0.005;

%% Collect data
count = 1;

while ~isequal(datestr(now,'mm/DD HH:MM'),stopTime)
    time(count) = datenum(clock); 
    fprintf(serialObject,'MEASURE:VOLTAGE:DC?'); % To measure current the command is MEASURE:CURRENT:DC?
    voltage(count) = fscanf(serialObject,'%f');  %#ok<SAGROW>
    set(plotHandle,'YData',voltage,'XData',time);
    set(figureHandle,'Visible','on');
    datetick('x','mm/DD HH:MM');
    
    pause(timeInterval);
    count = count +1;
end

%% Put the instrument in local mode
fprintf(serialObject,'SYSTEM:LOCAL');

%% Clean up the serial object
fclose(serialObject);
delete(serialObject);
clear serialObject;