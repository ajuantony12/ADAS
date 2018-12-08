% Example for blocked boxes
% Christoph Jurczyk

clearvars
close all

% Size of boxes
boxsize = 50; % in cm
% Max number of points in box
NMAXpointsBox = 10;

% Laser data
angle = linspace(0,pi,180);
pls_dist = [428, 431, 431, 388, 386, 388, 383, 383, 63, 53, 48, 58, 66, 328, 323, 313, 253, 253, 263, 283, 408, 418, 363, 268, 268, 273, 273, 278, 268, 278, 283, 278, 118, 88, 83, 83, 83, 83, 83, 88, 88, 83, 91, 91, 86, 86, 86, 86, 81, 81, 81, 81, 81, 78, 83, 126, 166, 166, 161, 161, 161, 156, 156, 156, 151, 151, 146, 146, 146, 146, 141, 141, 141, 141, 136, 136, 136, 136, 136, 131, 136, 131, 136, 136, 131, 134, 136, 131, 136, 134, 360, 428, 431, 431, 388, 386, 388, 383, 383, 63, 53, 48, 58, 66, 328, 323, 313, 253, 253, 263, 283, 408, 418, 363, 268, 268, 273, 273, 278, 268, 278, 283, 278, 118, 88, 83, 83, 83, 83, 83, 88, 88, 83, 91, 91, 86, 86, 86, 86, 81, 81, 81, 81, 81, 78, 83, 126, 166, 166, 161, 161, 161, 156, 156, 156, 151, 151, 146, 146, 146, 146, 141, 141, 141, 141, 136, 136, 136, 136, 136, 131, 136, 131, 136, 136, 131, 134, 136, 131, 136, ];
% Convert polar to cartesian
[pls_pts_x, pls_pts_y] = pol2cart(angle,pls_dist);
xmin = min(pls_pts_x);
xmax = max(pls_pts_x);
ymin = min(pls_pts_y);
ymax = max(pls_pts_y);
% Plot laser data
hold on
plot(pls_pts_x, pls_pts_y,'-', 'color',[0,0,0]+0.8)
plot(pls_pts_x, pls_pts_y,'b+')
grid
title(['Detection of blocked boxes (size: ' num2str(boxsize) 'cm x' num2str(boxsize) 'cm)'])
xlabel('x in cm')
ylabel('y in cm')

% Create point array
pointArray = initPoints(pls_pts_x, pls_pts_y);

% create boxes
boxArray = initBoxes(xmax, xmin, ymax, ymin, boxsize);

% Check points in boxes
boxArray = checkPointsInBoxes(boxArray,pointArray,NMAXpointsBox);

% Draw data
updateBoxes(boxArray);




% ######### Function #########

function boxArray = checkPointsInBoxes(boxArray,pointArray,NMAXpointsBox)
    [N_box_x, N_box_y] = size(boxArray);
     Npoints = length(pointArray);

     for n = 1:Npoints
        for y = 1:N_box_y
            for x = 1:N_box_x
                boxArray(x,y) = boxArray(x,y).checkPoint(pointArray(n), NMAXpointsBox);
            end%for
        end%for
     end%for
end%function


function pointArray = initPoints(x,y)
    Npoints = length(x);
    pointArray(1:Npoints) = clPoint;
    for n=1:Npoints
        pointArray(n) = clPoint(x(n),y(n));
    end%for

end%function

function boxArray = initBoxes(xmax, xmin, ymax, ymin, boxsize)
    N_box_x = ceil((xmax - xmin) / boxsize)+1;
    N_box_y = ceil((ymax - ymin) / boxsize);
    N_box_x_left = ceil((0 - xmin) / boxsize);
    x_start = -N_box_x_left*boxsize;
    boxArray(1:N_box_x, 1:N_box_y) = clBox;

    pos_y = ymin;
    for y = 1:N_box_y
        pos_x = x_start;
        for x = 1:N_box_x
            boxArray(x,y) = clBox(pos_x, pos_y, pos_x + boxsize, pos_y + boxsize);
            pos_x = pos_x + boxsize;
        end%for
        pos_y = pos_y + boxsize;
    end%for
end%function



function updateBoxes(boxArray)
    [N_box_x, N_box_y] = size(boxArray);
    % plot boxes
    for y = 1:N_box_y
        for x = 1:N_box_x
            boxArray(x,y).draw;
        end%for
    end%for
end%function