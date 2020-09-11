local TCE = require("data.scripts.tce.tce")

function checkColWithList(target, list)
    local collisions = {}
    for key in pairs(list) do
        if list[key].flags.solid == true and
            TCE.BoxAndBoxCollision(
            {x = target.pos.x, y = target.pos.y, w = target.width, h = target.height},
            {x = list[key].pos.x, y = list[key].pos.y, w = list[key].width, h = list[key].height}) then

            table.insert(collisions, list[key])
        end
    end

    return collisions
end

function checkColSpecific(target, specific)
    return TCE.BoxAndBoxCollision(
        {x = target.pos.x, y = target.pos.y, w = target.width, h = target.height},
        {x = specific.pos.x, y = specific.pos.y, w = specific.width, h = specific.height}
    )
end