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
    [11] = template_10x9_2p_lr,
    [12] = template_10x9_2p_lr,
    [13] = template_10x9_2p_lr,
    [14] = template_10x9_2p_lr,
    [15] = template_10x9_2p_lr,
    [16] = template_10x9_2p_lr,
    [17] = template_10x9_2p_lr,
    [18] = template_10x9_2p_lr,
    [19] = template_10x9_2p_lr 
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
               if force == "auto" then
                   force = 0
               end
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
        elseif map[i] == 0 then
            table.insert(Map,1)
        else
            table.insert(Map,formWithId[map[i]])
        end
    end
    return Map
end


--get force info 
function GetForce(mapId)
    local Force = {}
    local force = GetForceInfo(mapId)
    local forceWithId = GetForce2Id(mapId)
    for i=1,#force do
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

function GetActForceIds(mapId)
    local force = GetForce(mapId)
    local forceWithId = GetForce2Id(mapId)
    local forceId = {}
    for i=1,#force do
        if force[i] ~= 0 then 
            if #forceId == 0 then
                table.insert(forceId,force[i])
            else
                local flag = 0
                for j=1,#forceId do
                    if forceId[j] == force[i] then
                        flag = 1
                    end
                end
                if flag == 0 then
                    table.insert(forceId,force[i])
                end
            end
        end
    end
    return forceId
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
    ["auto"] = 0,
    ["left"]  = 1,
    ["right"] = 2,
    ["up"]  = 3,
    ["down"] = 4
}


function GetSoldierObject(mapId)
    local layers = GetLayers(mapId);
    for i=1,#layers do
        if layers[i].type=="objectgroup" and layers[i].name=="soldier" then
            return layers[i]
        end
    end
end

--获得hero这个图块的
function GetHeroTile(mapId)
    local tilesets = GetTileSets(mapId)
    for i=1,#tilesets do
        if tilesets[i].name == "hero" then
            return tilesets[i]
        end
    end
end

--获得hero图块的宽度
function GetHeroTileWidth(mapId)
    local heroTile = GetHeroTile(mapId)
    return heroTile.tilewidth
end


--获得hero图块的高度
function GetHeroTileHeight(mapId)
    local heroTile = GetHeroTile(mapId)
    return heroTile.tileheight
end


--把图块id和fighterId对应起来
function GetSoldier2Id(mapId)
    local heroTile = GetHeroTile(mapId)
    local soldier2id = {}
    local firstgid = heroTile.firstgid
    local tiles = heroTile.tiles
    for j=1,#tiles do
        local id = tiles[j].id
        local properties = tiles[j].properties
        local fighterId = properties["heroId"]
        soldier2id[firstgid+id] = fighterId
    end
    return soldier2id
end

function GetTileHeight(mapId)
    return name2table[mapId].tileheight
end

function GetTileWidth(mapId)
    return name2table[mapId].tilewidth
end

--获得野怪(npc)信息
function ParseSoldier(mapId)
    local soldierobjects = GetSoldierObject(mapId)
    local soldiers = {}
    if soldierobjects == nil then 
        return soldiers
    end
    local objects = soldierobjects.objects
    if objects == nil then 
        return soldiers
    end

    local soldier2id = GetSoldier2Id(mapId)
    local tilewidth = GetTileWidth(mapId)
    local tileheight = GetTileHeight(mapId)
    local height = GetMapHeight(mapId)
    for i=1,#objects do
        local x = objects[i].x
        local y = objects[i].y
        local gid = objects[i].gid
        --把世界坐标转换为地图坐标
        local fighterId = soldier2id[gid]
        local posx = 0
        local posy = (y-tileheight)/(0.5*tileheight)
        if posy%2 == 0 then
            posx = x/tilewidth
        else
            posx = (x-0.5*tilewidth)/tilewidth
        end
        table.insert(soldiers,{fighterId,posx,posy})
    end
    return soldiers
end


function GetForceDirection(mapId)
    local direction = {}
    local tiles = GetTiles(mapId)
    for i=1,#tiles do 
        local properties = tiles[i].properties
        if properties["force"] == "auto" then
            table.insert(direction,0)
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

function GetAllSoldiers()
    local soldierAll = {}
    for i=1,#name2table do
        local soldiers = ParseSoldier(i)
        table.insert(soldierAll,soldiers)
    end
    return soldierAll
end


function GetAllActForceIds()
    local forceIdAll = {}
    for i=1,#name2table do
        local forceIds = GetActForceIds(i)
        table.insert(forceIdAll,forceIds)
    end
    return forceIdAll
end
