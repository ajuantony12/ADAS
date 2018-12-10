classdef clPoint
    % Description: Class for cartesian point
    % Author: Christoph Jurczyk
    
    properties
        x
        y
    end%properties
    
    methods
        function obj = clPoint(varargin)
            % Intialization of object
            if nargin == 2
                obj.x = varargin{1,1};
                obj.y = varargin{1,2};
            end%if
            
        end%function
    end%methods
end%classdef

