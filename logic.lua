
function tryMove(x0, y0, name0, x1, y1, name1)
    local success = setName(simulation, x1, y1, name0)
    if success then
        setName(simulation, x0, y0, name1)
        setUpdate(simulation, x1, y1)
        return true
    end
    return false
end

empty = {
    name = "empty",
    r=0,
    g=0,
    b=0,
    a=0,
 }

sand = {
    name = "sand",
    r = 255,
    g = 255,
    b = 0,
    a = 255,
    update = function(x, y)
        local name = getName(simulation, x, y + 1)
        if name == empty.name or name == nil then
            tryMove(x, y, sand.name, x, y + 1, empty.name)
        elseif name == water.name or name == smoke.name then
            tryMove(x, y, sand.name, x, y + 1, name)
        else
            local dirs = {1, -1}
            if math.random(0, 1) == 1 then
                dirs[1], dirs[2] = dirs[2], dirs[1]
            end

            for _, dir in ipairs(dirs) do
                if getName(simulation, x + dir, y + 1) == empty.name and getName(simulation, x + dir, y) == empty.name then
                    tryMove(x, y, sand.name, x + dir, y + 1, empty.name)
                    break
                end
            end
        end
    end
}

water = {
    name = "water",
    r = 0,
    g = 0,
    b = 255,
    a = 127,
    update = function(x, y)
        local name = getName(simulation, x, y + 1)
        if name == empty.name or name == nil then
            tryMove(x, y, water.name, x, y + 1, empty.name)
        else
            local dirs = {1, -1}
            if math.random(0, 1) == 1 then
                dirs[1], dirs[2] = dirs[2], dirs[1]
            end

            for _, dir in ipairs(dirs) do
                if getName(simulation, x + dir, y) == empty.name then
                    tryMove(x, y, water.name, x + dir, y, empty.name)
                    break
                end
            end
        end
    end
}

smoke = {
    name = "smoke",
    r=64,
    g=64,
    b=64,
    a=127,
    update = function(x, y)
        local name = getName(simulation, x, y - 1)
        if name == empty.name or name == water.name or name == nil then
            tryMove(x, y, smoke.name, x, y - 1, name)
        else
            local dirs = {1, -1}
            if math.random(0, 1) == 1 then
                dirs[1], dirs[2] = dirs[2], dirs[1]
            end

            for _, dir in ipairs(dirs) do
                if getName(simulation, x + dir, y) == empty.name then
                    tryMove(x, y, smoke.name, x + dir, y, empty.name)
                    break
                end
            end
        end
    end
}

stone = {
    name = "stone",
    r=127,
    g=127,
    b=127,
    a=255,
}

wood = {
    name = "wood",
    r=64,
    g=48,
    b=24,
    a=255,
}

registerParticle(simulation, empty)
registerParticle(simulation, sand)
registerParticle(simulation, water)
registerParticle(simulation, stone)
registerParticle(simulation, smoke)
registerParticle(simulation, wood)
