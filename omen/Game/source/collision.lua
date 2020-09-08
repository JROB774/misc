--[[
Collision script contains a multitude of functions used to calculate
whether objects of different types have collided. Currently, these types
inclue: points, and quads.

Functions return true if collision occurred, false if not.
--]]

-- the global collision namespace
Collision = {}
Collision.__index = Collision

-- detects collision beterrn a point and point
function Collision.pointAndPoint (_point1, _point2)
	return (
		(_point1.x == _point2.x) and
		(_point1.y == _point2.y)
	)
end

-- detects collision between a point and quad
function Collision.pointAndQuad (_point, _quad)
	return (
		(_point.x >= _quad.x) and
		(_point.y >= _quad.y) and
		(_point.x < (_quad.x + _quad.w)) and
		(_point.y < (_quad.y + _quad.h))
	)
end

-- detects collision between a quad and quad
function Collision.quadAndQuad (_quad1, _quad2)
	return (
		(_quad1.x < (_quad2.x + _quad2.w)) and
		((_quad1.x + _quad1.w) > _quad2.x) and
		(_quad1.y < (_quad2.y + _quad2.h)) and
		((_quad1.y + _quad1.h) > _quad2.y)
	)
end