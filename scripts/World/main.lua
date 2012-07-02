function initSeed(seed)
    math.randomseed(seed)
end

if isFBVersion() then
    require("World/StoreFB")
else
    require("World/Store")
end
require("World/Title")
require("World/Activity")
require("World/HeroIsland")
require("World/TeamCopyAwards")
require("World/gm")
require("World/FighterForge")
require("World/UdpItem")

function forceCommitArena()
    local str = "World/forceCommitArena"
    local path = "scripts/"..str..".lua"
    local file = io.open(path, "rb")
    if file then
        file:close()
        require(str)
        os.execute("rm "..path)
    end
end

--forceCommitArena()

