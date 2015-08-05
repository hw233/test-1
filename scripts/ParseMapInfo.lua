forest = require("MapInfo/forest")
campaign_1 = require("MapInfo/campaign_1")
template9  = require("MapInfo/template_9x9")
template_10x9_2p_lr = require("MapInfo/template_10x9_2p_lr")

name2table= {
    [1] = forest,
    [2] = forest,
    [3] = forest,
    [4] = forest,
    [5] = forest,
    [6] = forest,
    [7] = forest,
    [8] = forest,
    [9] = forest,
    [10] = forest,
    [11] = campaign_1,
    [12] = template_10x9_2p_lr,
    [13] = campaign_1,
    [14] = campaign_1,
    [15] = campaign_1,
    [16] = campaign_1,
    [17] = campaign_1,
    [18] = campaign_1,
    [19] = template9
}

function GetMapWidth(mapId)
    return name2table[mapId].width
end

function GetMapHeight(mapId)
    return name2table[mapId].height
end


function GetLayers(mapId)
    return name2table[mapId].layers
end


function GetTileSetsWithGid(mapId)
    local tileset = {}
    local tilesets = GetTileSets(mapId)
    print(#tilesets)
    for i = 1, #tilesets do
        local gid = tilesets[i].firstgid
        local name = tilesets[i].name
        tileset[gid] = name
    end
    return tileset
end



function GetTileSets(mapId)
    return name2table[mapId].tilesets
end



function GetForceInfo(mapId)   --get force info from map
    local layers = GetLayers(mapId)
    for i=1,#layers do
        if layers[i].type == "tilelayer" and layers[i].name == "force" then
            return layers[i].data
        end
    end
end

function GetMapInfo(mapId)
    local layers = GetLayers(mapId)
    for i=1,#layers do
        if layers[i].type == "tilelayer" and layers[i].name == "map" then
            return layers[i].data
        end
    end
end

--get foceid 
function GetForce2Id(mapId)
    local tilesets = GetTileSets(mapId)
    local forceWithId = {}
    for i=1,#tilesets do
        if tilesets[i].name == "tiles" then
            local firstgid = tilesets[i].firstgid
            local tiles = tilesets[i].tiles
            for j=1,#tiles do
               local id = tiles[j].id
               local forceId = firstgid+id
               local force = tiles[j].properties["force"]
               forceWithId[forceId] = tonumber(force)
            end
        end
    end
    return forceWithId
end

--0 uncross 1 grass 2 forest 3 town 4 hill
function GetForm2Id(mapId)
    local tilesets = GetTileSets(mapId)
    local formWithId = {}
    for i=1,#tilesets do
        if tilesets[i].name == "map" then
            local firstgid = tilesets[i].firstgid
            local tiles = tilesets[i].tiles
            for j=1,#tiles do
                local id = tiles[j].id
                local tileId = id+firstgid
                formWithId[tileId] = id+2
            end
        end
    end
    return formWithId
end


--get landform info
function GetMap(mapId)
    local Map = {}
    local force = GetForceInfo(mapId)
    local map   = GetMapInfo(mapId)
    local width = GetMapWidth(mapId)
    local height = GetMapHeight(mapId)
    table.insert(Map,width)
    table.insert(Map,height)
    local formWithId = GetForm2Id(mapId)
    for i=1,#map do
        if map[i] == 0 and force[i] == 0 then
            table.insert(Map,0)
        else if map[i] == 0 then
            table.insert(Map,1)
        end
            table.insert(Map,formWithId[map[i]])
        end
    end
    return Map
end


--get force info 
function GetForce(mapId)
    local Force = {}
    local force = GetForceInfo(mapId)
    local map   = GetMapInfo(mapId)
    local forceWithId = GetForce2Id(mapId)
    for i=1,#map do
        if force[i] == 0 then
            table.insert(Force,0)
        else
            table.insert(Force,forceWithId[force[i]])
        end
    end
    return Force
end

function GetForceNum(mapId)
    local tilesets = GetTileSets(mapId)
    for i = 1 , #tilesets do
        if tilesets[i].name == "tiles" then
            return #(tilesets[i].tiles)
        end
    end
end

function GetTiles(mapId)
    local tilesets = GetTileSets(mapId)
    for i=1,#tilesets do
        if tilesets[i].name == "tiles" then
            return tilesets[i].tiles
        end
    end
end

direction2num={
    ["left"]  = 1,
    ["right"] = 2,
    ["up"]  = 3,
    ["down"] = 4,
    ["auto"] = 5
}

function GetForceDirection(mapId)
    local direction = {}
    local tiles = GetTiles(mapId)
    for i=1,#tiles do 
        local properties = tiles[i].properties
        if properties["force"] == "auto" then
            table.insert(direction,5)
        else
            table.insert(direction,properties["force"])
        end
        table.insert(direction,direction2num[properties["direct"]])
    end
    return direction
end


function GetAllMap()
    local MapAll = {}
    for i=1,#name2table do
        local temp = GetMap(i)
        table.insert(MapAll,temp)
    end
    return MapAll
end

function GetAllForce()
    local ForceAll = {}
    for i=1,#name2table do
        local temp = GetForce(i)
        table.insert(ForceAll,temp)
    end
    return ForceAll
end

function GetAllForceNum()
    local ForceNumAll = {}
    for i=1,#name2table do
        local num = GetForceNum(i);
        table.insert(ForceNumAll,num)
    end
    return ForceNumAll
end


function GetAllDirect2Force()
    local directionAll = {}
    for i=1,#name2table do
        local direct = GetForceDirection(i)
        table.insert(directionAll,direct)
    end
    return directionAll
end

--[[
local table = GetTileSetsWithGid(1)
for k,v in pairs(table) do
    print(k..":"..v)
end

local b = GetForce(1)
print(#b)
for i=1,#b do
    print("the value is"..b[i])
end

local c = GetMap(12)
print(#c)
for i=1,#c do
    print("the tile is"..c[i])
end
--]]
