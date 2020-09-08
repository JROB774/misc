local TCE = require("data.scripts.tce.tce")

Entity = {}
Entity.__index = Entity

-- Default values!
Entity.pos = {x = 0, y = 0}
Entity.width = 0
Entity.health = 5
Entity.attack = 1
Entity.angle = 0
Entity.solid = false
Entity.renderOn = true
Entity.updateOn = true
Entity.texture = nil


setmetatable(Entity, {__call = function (cls, ...) return cls.new(...) end})

function Entity.new(x, y, width, height, health, attack, speed, angle, solid, renderOn, updateOn, texture, insertIntoTable)
    --insertIntoTable = insertIntoTable or "enemy"
	local self = setmetatable({
		pos = {x = x, y = y};
        xTarget = x;
        yTarget = y;
        width = width;
        height = height;
        health = health;
        attack = attack;
        speed = {x = speed, y = speed};
    	angle = angle;
        angleTarget = angle;
    	solid = solid;
    	renderOn = renderOn;
    	updateOn = updateOn;
    	texture = texture;
        tag = insertIntoTable;
        collisions = {};
        lastVelocity = {x = 0.00001, y = 0};
        lastAttack = nil;
	}, Entity)

    if insertIntoTable == "tile" then
        table.insert(tileList, self)
    elseif insertIntoTable == "player" then
        print ("playerCreated")
    else
        table.insert(entityList, self)
    end

    table.insert(renderList, self)

    self.defenderPresent = false;
    self.state = ""

    return self
end

function Entity:update(dt)
	-- ...
end

function Entity:render()
    if (self.lastVelocity.x > 0) then
	   TCE.RenderTexture(self.texture, {x = self.pos.x, y = self.pos.y, angle = 0})
    elseif (self.lastVelocity.x < 0) then
        TCE.RenderTexture(self.texture, {x = self.pos.x, y = self.pos.y, angle = 0, flip = TCE.RenderFlips.HORZ})
    end
end

function Entity:checkColTiles()
    self.collisions = {}
    for key in pairs(tileList) do
        if tileList[key].solid == true and
            TCE.BoxAndBoxCollision(
            {x = self.pos.x, y = self.pos.y, w = self.width, h = self.height},
            {x = tileList[key].pos.x, y = tileList[key].pos.y, w = tileList[key].width, h = tileList[key].height}) then

            --BREAKS IF MOVE WITH VELOCITY
            self:movementRebound(tileList[key])
            table.insert(self.collisions, tileList[key])
        end
    end
end

function Entity:checkEntityTiles()
    self.collisions = {}
    for key in pairs(entityList) do
        if entityList[key].solid == true and
            TCE.BoxAndBoxCollision(
            {x = self.pos.x, y = self.pos.y, w = self.width, h = self.height},
            {x = entityList[key].pos.x, y = entityList[key].pos.y, w = entityList[key].width, h = entityList[key].height}) then

            table.insert(self.collisions, entityList[key])
        end
    end
end

function Entity:checkColSpecific(specific)
    return TCE.BoxAndBoxCollision(
        {x = self.pos.x, y = self.pos.y, w = self.width, h = self.height},
        {x = specific.pos.x, y = specific.pos.y, w = specific.width, h = specific.height}
    )
end

function Entity:movementRebound(collided)
    -- @TODO:  maybe make this a proper high constant at some point???
    xInterp = 1000000
    yInterp = 1000000

    if (self.pos.x < collided.pos.x) then xInterp = -((self.pos.x + self.width) - collided.pos.x)
    else xInterp = ((collided.pos.x + collided.width) - self.pos.x) end

    if (self.pos.y < collided.pos.y) then yInterp = -((self.pos.y + self.height) - collided.pos.y)
    else yInterp = ((collided.pos.y + collided.height) - self.pos.y) end

    xInterp = TCE.Round(xInterp)
    yInterp = TCE.Round(yInterp)

    if (math.abs(xInterp) > math.abs(yInterp)) then
        self.pos.y = self.pos.y + yInterp
        self.speed.y = 0.0
    else
        self.pos.x = self.pos.x + xInterp
        self.speed.x = 0.0
    end
end

function Entity:lookAtVector(target, inital)
    length = math.sqrt((target.x - inital.x )^2 + (target.y - inital.y)^2)
    xDiff = (target.x - inital.x) / length
    yDiff = (target.y - inital.y) / length

    return {x = xDiff, y = yDiff, length = length}

end

function Entity:dealDmg(dmg)
    self.health = self.health - dmg
    if self.health < 1 then
        self.solid = false;
        self.renderOn = false;
        self.updateOn = false;
    end
end
