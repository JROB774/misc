--[[
Utility script contains multiple helper functions and tools that can
be used throughout the application code-base to aide development.
--]]

-- the global utility namespace is created
Utility = {}
Utility.__index = Utility

--[[
This function takes in a table of information and makes it immutable/constant.
This means that if any other part of the code attempts to change the values
within the table, an error will be thrown. This is very useful for ensuring
that certain constant values do not change over the course of the program,
which could cause errors.
--]]

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

--[[
This function takes in an angle (in degrees) and caps it so that it is
within 0.0 and 360.0 degrees. WARNING: As it is expected that this function
is called reguarly on angles to ensure they stay in range, it will no
longer accurately wrap/cap the angle's value if it is over 360 * 2 degrees,
or under -(360 * 2) degrees.
--]]

function Utility.capAngle (_angle)
	if _angle < 0.0 then return _angle + 360.0
	elseif _angle > 360.0 then return _angle - 360.0
	else return _angle end
end

--[[
This function takes in a value as well as a minimum and maximum boundary.
It then checks if the value is within these bounds (inclusive), returning
true if so and false if not. The function is used to make comparisons such
as these look much cleaner and readable within the code.
--]]

function Utility.inBounds (_val, _min, _max)
	if _val >= _min and _val <= _max then return true
	else return false end
end

--[[
This function takes two points and calculated the length/distance between
those two points, returning the value after the calculation is complete. This
is useful for determining the length of rays and as a result how to project
wall slices, etc.
--]]

function Utility.length (_x1, _y1, _x2, _y2)
	return math.sqrt(math.pow((_x1 - _x2), 2) + math.pow((_y1 - _y2), 2))
end

-- an enumeration of four directional values
Utility.dir = { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 }
Utility.dir = Utility.immutable(Utility.dir)