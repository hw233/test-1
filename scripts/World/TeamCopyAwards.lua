
--  奖励
local awards = {
    -- 物品ID 物品个数

 [1] = {
        {11,1}
        {11,2}
        {400,1}
        {400,2}
        {519,4}
        {519,6}
        {526,4}
        {526,6}
    },
    [2] = {
        {11,2}
        {11,4}
        {400,2}
        {400,3}
        {520,4}
        {520,6}
        {527,4}
        {527,6}
    },
    [3] = {
        {11,4}
        {11,8}
        {400,2}
        {400,4}
        {521,4}
        {521,6}
        {528,4}
        {528,6}
    },
    [4] = {
        {11,6}
        {11,12}
        {400,3}
        {400,6}
        {522,4}
        {522,6}
        {529,4}
        {529,6}
    },
    [5] = {
        {11,8}
        {11,16}
        {400,4}
        {400,8}
        {523,4}
        {523,6}
        {530,4}
        {530,6}
    },
    [6] = {
        {11,10}
        {11,20}
        {400,5}
        {400,10}
        {524,4}
        {524,6}
        {531,4}
        {531,6}
    },

}


function loadTeamCopyAwardsCfg()
    clearTeamCopyAwardCfg();
    for k1,v1 in pairs(awards)
    do
        for k,v in pairs(v1)
        do
            addTeamCopyAwardCfg(k1, v[1], v[2]);
        end
    end
end

