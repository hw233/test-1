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
