classdef clBox
    % Description: Class for boxes in cartesian coordinate
    % Author: Christoph Jurczyk
    %
    %          x...........x <- P2
    %          .           .
    %          .           .
    %    P1 -> x...........x
    %
    
    properties
        P1
        P2
        NjammedPoint = 0;
        NMaxjammedPoints = 10;
        jammed = false;
    end%properties
    
    methods
        function obj = clBox(varargin)
            % Intialization of object
            if nargin == 2
                % Expected input: 2x clPoint
                P1 = varargin{1,1};
                P2 = varargin{1,2};
                if(~isa(P1,'clPoint') || ~isa(P2,'clPoint'))
                    error('Please chek class type of P1 and P2!')
                end%for
            elseif nargin == 4
                % Expected input: x1, y1, x2, y2
                P1 = clPoint(varargin{1,1}, varargin{1,2});
                P2 = clPoint(varargin{1,3}, varargin{1,4});    
            else
                %error('Please check input');
                return;
            end%if
            obj.P1 = P1;
            obj.P2 = P2;
        end%function
        
        % Function to draw boxes in plot
        function draw(obj)
            % select color
            box_color = [1 0 0 0.1];
           
            % plot
            rectangle('Position',...
                      [obj.P1.x obj.P1.y...
                       abs(obj.P2.x-obj.P1.x) abs(obj.P2.y-obj.P1.y)],...
                       'FaceColor',box_color)
        end%function
        
        % Function to check if Point is in box
        function result = checkPoint(obj, point)
            result = false;
            if((point.x >= obj.P1.x) && (point.x < obj.P2.x))
                if((point.y >= obj.P1.y) && (point.y < obj.P2.y))
                    % point in box
                    result = true;
                end%if
            end%if
   
        end%function
  
        
    end%methods
end%classdef

