classdef clPoint
    % Class for cartesian point
    
    properties
        x
        y
    end
    
    methods
        function obj = clPoint(varargin)
            % Intialization of object
            if nargin == 2
                obj.x = varargin{1,1};
                obj.y = varargin{1,2};
            end%if
            
        end%function
    end
end

