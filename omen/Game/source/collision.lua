--[[
Collision script contains a multitude of functions used to calculate
whether objects of different types have collided. Currently, these types
include: points, and quads.

Functions return true if collision occurred, false if not.
--]]

-- the global collision namespace is created
Collision = {}
Collision.__index = Collision

--[[
This function takes in two points and determines if they are equal to one
another, if so then a collision has occurred.
--]]

function Collision.pointAndPoint (_point1, _point2)
	return (
		(_point1.x == _point2.x) and
		(_point1.y == _point2.y)
	)
end

--[[
This function takes in a point and quad and checks to see if the point
exists anywhere within the boundaries of the quad, if so then a collision
has occurred.
--]]

function Collision.pointAndQuad (_point, _quad)
	return (
		(_point.x >= _quad.x) and
		(_point.y >= _quad.y) and
		(_point.x < (_quad.x + _quad.w)) and
		(_point.y < (_quad.y + _quad.h))
	)
end

--[[
This function takes in two quads and checks to see if any portion of one
quad is overlapping with a portion of the other quad, if so a collision
has occurred.
--]]

function Collision.quadAndQuad (_quad1, _quad2)
	return (
		(_quad1.x < (_quad2.x + _quad2.w)) and
		((_quad1.x + _quad1.w) > _quad2.x) and
		(_quad1.y < (_quad2.y + _quad2.h)) and
		((_quad1.y + _quad1.h) > _quad2.y)
	)
end