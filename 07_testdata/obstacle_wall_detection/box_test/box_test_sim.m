% Example for blocked boxes
% Christoph Jurczyk

clearvars
clf

% Size of boxes
boxsize = 40; % in cm
% Max number of points in box
NMAXpointsBox = 2;
% Max laser distance
MAX_DIST = 500;

% Laser data
load('testdata_sim.mat')
pls_pts_x = x*100;
pls_pts_y = y*100;
xmin = min(pls_pts_x);
xmax = max(pls_pts_x);
ymin = min(pls_pts_y);
ymax = max(pls_pts_y);
pls_pts_y = pls_pts_y - ymin;
ymin = min(pls_pts_y);
% Plot laser data
hold on
plot(pls_pts_x, pls_pts_y,'-', 'color',[0,0,0]+0.8)
plot(pls_pts_x, pls_pts_y,'b+')
title(['Detection of blocked boxes (size: ' num2str(boxsize) 'cm x ' num2str(boxsize) 'cm)'])
xlabel('x in cm')
ylabel('y in cm')

% Create point array
pointArray = initPoints(pls_pts_x, pls_pts_y);

% create boxes
boxArray = initBoxes(xmax, xmin, MAX_DIST, ymin, boxsize);

% Check points in boxes
boxArray = checkPointsInBoxes(boxArray,pointArray,NMAXpointsBox);

% Draw data
updateBoxes(boxArray);
PLSsize = plotPLS(pls_pts_x);

% Limit Axes
xlim([boxArray(1,1).P1.x boxArray(end,1).P2.x])
ylim([(boxArray(1,1).P1.y - PLSsize) MAX_DIST+2*PLSsize])
axis image % same scale on x- and y-axis


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


function PLSsize = plotPLS(pls_pts_x)
    % calculate size of PLS
    PLSsize =  round(0.03*max([abs(min(pls_pts_x)) max(pls_pts_x)]));
    if(PLSsize < 10)
        PLSsize = 10;
    end%if

    rectangle('Position',...
              [-PLSsize/2 -PLSsize PLSsize PLSsize],...
              'FaceColor',[0 0 1])
end%function