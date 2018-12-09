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
            if obj.jammed
                box_color = [1 0 0 0.1];
            else
                box_color = [0 1 0 0.1];
            end%if
            
            % plot
            rectangle('Position',...
                      [obj.P1.x obj.P1.y...
                       obj.P2.x-obj.P1.x obj.P2.y-obj.P1.y],...
                       'FaceColor',box_color)
            x_text = obj.P1.x + (obj.P2.x - obj.P1.x)/2;
            y_text = obj.P1.y + (obj.P2.y - obj.P1.y)/2;
            txt = num2str(obj.NjammedPoint);
            text(x_text,y_text,txt)
        end%function
        
        % Function to check if Point is in box
        function obj = checkPoint(obj, point, NMaxjammedPoints)
            if(~isa(point,'clPoint'))
                    error('Please chek class type of point')
            end%for
            
            if((point.x >= obj.P1.x) && (point.x < obj.P2.x))
                if((point.y >= obj.P1.y) && (point.y < obj.P2.y))
                    % point in box
                    obj.NjammedPoint = obj.NjammedPoint + 1;
                end%if
            else
                % point not in box
            end%if
            obj.NMaxjammedPoints = NMaxjammedPoints;
            if obj.NjammedPoint >= obj.NMaxjammedPoints
                obj.jammed = true;
            else
                obj.jammed = false;
            end%if
        end%function
  
        
    end%methods
end%classdef

