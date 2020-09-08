--[[
Utility script contains multiple helper functions and tools that can
be used throughout the application code-base to aide development.
--]]

-- the global utility namespace
Utility = {}
Utility.__index = Utility

-- makes a given table immutable/constant
function Utility.immutable (_table)
	return setmetatable({}, {
		__index = _table,
		__newindex = function (_tbl, _key, _value)
			error("Attempting to change immutable value " ..
				tostring(_key) .. " to " .. tostring(_value), 2
			)
		end
	})
end

-- caps an angle between the values 0.0 and 360.0
function Utility.capAngle (_angle)
	if _angle < 0.0 then return _angle + 360.0
	elseif _angle > 360.0 then return _angle - 360.0
	else return _angle end
end

-- checks to see if a value is within specified boundaries
function Utility.inBounds (_val, _min, _max)
	if _val >= _min and _val <= _max then return true
	else return false end
end

-- calculates the length between two points
function Utility.length (_x1, _y1, _x2, _y2)
	return math.sqrt(math.pow((_x1 - _x2), 2) + math.pow((_y1 - _y2), 2))
end

-- enumeration of directional values
Utility.dir = { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 }
Utility.dir = Utility.immutable(Utility.dir)

-- enumeration of compass-based directional values
Utility.compass = { N = 0, NE = 1, E = 2, SE = 3, S = 4, SW = 5, W = 6, NW = 7 }
Utility.compass = Utility.immutable(Utility.compass)