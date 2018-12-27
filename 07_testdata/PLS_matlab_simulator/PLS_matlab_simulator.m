clearvars

% Laser parameters
pls_res = 1; % resolution in deg
pls_dof = 180; % degree of freedome in deg
pls_n = pls_dof / pls_res; % number of measurements
pls_max_dist = 5; % in m
pls_x = 0; % in m
pls_y = 0; % in m
pls_angle = 0; % in deg
% Resolution of simulation polar line
sim_grid = 5e-2; % in m


% Initialize Map 
% Add some obstacles to the map
boxes        = clBox(1,2,1.5,3); % box
boxes(end+1) = clBox(-2,8.2,-1.5,9); % box
boxes(end+1) = clBox(-2,11,-1.75,11.5); % box
boxes(end+1) = clBox(2.5,0,2.75,15); % wall
boxes(end+1) = clBox(-2.75,0,-2.5,15); % wall


% Intialize Route
% Route points: x, y
rte_points = [
	0,		0;
	0, 		4;
	1,      8;
	1,      10;
];
hold on
rte_data = calcRouteData(rte_points, 0.1);

% Set plot range
xlim_value = [min(rte_points(:,1))-pls_max_dist max(rte_points(:,1)) + pls_max_dist];
ylim_value = [min(rte_points(:,2)) max(rte_points(:,2)) + pls_max_dist];


for n = 1:length(rte_data)
    % Initialize Plot
    clf
    hold on
    grid
    axis equal
    xlim(xlim_value);
    ylim(ylim_value);

    pls_x = rte_data(n).x;
    pls_y = rte_data(n).y;
    pls_angle = rte_data(n).angle;
    
    % Plot obstacles
    plotBoxes(boxes)

    % Initialize PLSdataArray with max distance points
    PLSdataArray = initPoints(pls_n, pls_res, pls_x, pls_y, pls_angle, pls_max_dist);

    % Check colidation of PLS beam
    PLSdataArray = calcPLSdata(PLSdataArray, pls_x, pls_y, boxes, sim_grid, pls_max_dist);

    % Plot PLS data
    PLSdataPlot(PLSdataArray, pls_x, pls_y);
    
    % Wait for plot
    pause(eps)
end%for




% -----------------------------------------------------------------------------

% Function to calculate PLS measurements with map
function PLSdataArray = calcPLSdata(PLSdataArray, pls_x, pls_y, boxes, sim_grid, max_dist)
    for n = 1:length(PLSdataArray)
        angle = PLSdataArray(n).angle;
        for dist = 0:sim_grid:max_dist
            x = dist*cos(deg2rad(angle)) + pls_x;
            y = dist*sin(deg2rad(angle)) + pls_y;
            %plot(x,y,'r*') % Plot checked point for debugging
            P_check = clPoint(x,y);

            result = CheckPinBoxes(P_check, boxes);

            if result
                PLSdataArray(n) = clPointPLS(x,y,angle);
                break;
            end%if

        end%for
    end%for
end%function

% Function to check if point is blocked by obstacle
function colidation = CheckPinBoxes(P_check, boxes)
 colidation = false;
 
    for n = 1:length(boxes)
        result = boxes(n).checkPoint(P_check);
        if result == true
            colidation = true;
            break;
        end%if
    end%for
        
end%for
 

% Function to initialize PLS points with max distance points
function PLSdataArray = initPoints(pls_n, pls_res, pls_x, pls_y, pls_angle, max_dist)
    PLSdataArray(1:pls_n) = clPointPLS;
    for n = 1:length(PLSdataArray)
        angle = (n-1) * pls_res + pls_angle;
        x = max_dist*cos(deg2rad(angle)) + pls_x;
        y = max_dist*sin(deg2rad(angle)) + pls_y;
        PLSdataArray(n) = clPointPLS(x,y,angle);
    end%for
end%function

% Plot PLS data
function PLSdataPlot(PLSdataArray, pls_x, pls_y)
    x = [];
    y = [];
    
    for n = 1:length(PLSdataArray)
        x(end+1) = PLSdataArray(n).x;
        y(end+1) = PLSdataArray(n).y;
    end%for
    
    
    plot(x,y,'b+-')
    plot([PLSdataArray(1).x PLSdataArray(end).x],[PLSdataArray(1).y PLSdataArray(end).y],'b-')
    plot(pls_x, pls_y , 'b*')
end%function

% Plot boxes
function plotBoxes(boxes)
    for n = 1:length(boxes)
        boxes(n).draw
    end%for
end%function
