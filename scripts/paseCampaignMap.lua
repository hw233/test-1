campaignMap1 = require("CampaignMap/CampaignMap1")

campaignMaplist = {
    [1] = campaignMap1
}

print(type(campaignMap1))

print(campaignMap1[1][1])
print(campaignMap1[1][2])

function GetAllBattleMap()
    return campaignMaplist
end
