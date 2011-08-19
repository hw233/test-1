--22 绿色斗剑场宝箱
--23 蓝色斗剑场宝箱
--24 紫色斗剑场宝箱
--25 橙色斗剑场宝箱

function RunAthleticsEvent(row, atker, defer, win)
    if 0 ~= _GameActionLua:getAthleticsFirst4Rank(atker, 0x80) then    --第一次竞技场挑战
        local package = atker:GetPackage();
        package:AddItem(22, 1, 1);
        _GameActionLua:AddAthleticsEvent(row, atker, defer, 12, 1, 22);
        _GameActionLua:setAthleticsFirst4Rank(atker, 0x80);
        return;
    end

    local cond = 0;
    local itemId = 0;
    local itemCount = 0;
    if win == 2 then
        if row == 1 and _GameActionLua:getAthleticsExtraChallenge(atker) ~= 0 then --放弃特殊挑战
            cond = 28;
            itemId = 23;
            itemCount = 1;
            _GameActionLua:setAthleticsExtraChallenge(atker, 0);
        end

    elseif win == 1 then
        local package = atker:GetPackage();

        if row == 1 and _GameActionLua:getAthleticsExtraChallenge(atker) ~= 0 then --特殊挑战胜利
            cond = 26;
            itemId = 24;
            itemCount = 1;
            _GameActionLua:setAthleticsExtraChallenge(atker, 0);
        elseif 1 == _GameActionLua:getAthleticsRank(atker) and row == 1 then
            if 0 == _GameActionLua:getAthleticsFirst4Rank(atker, 0x1) then     --第一次成为竞技场第一
                cond = 1;
                itemId = 25;
                itemCount = 5;
                _GameActionLua:setAthleticsFirst4Rank(atker, 0x1);
            else
                cond = 2;
            end

        elseif 2 == _GameActionLua:getAthleticsRank(atker) and row == 1 then
            if 0 == _GameActionLua:getAthleticsFirst4Rank(atker, 0x2) then     --第一次杀入竞技场二强
                cond = 3;
                itemId = 25;
                itemCount = 3;
                _GameActionLua:setAthleticsFirst4Rank(atker, 0x2);
            else
                cond = 4;
            end

        elseif 3 == _GameActionLua:getAthleticsRank(atker) and row == 1 then
            if 0 == _GameActionLua:getAthleticsFirst4Rank(atker, 0x4) then    --第一次杀入竞技场三强
                cond = 5;
                itemId = 25;
                itemCount = 2;
                _GameActionLua:setAthleticsFirst4Rank(atker, 0x4);
            else
                cond = 6;
            end

        elseif 11 > _GameActionLua:getAthleticsRank(atker) and row == 1 then
            if 0 == _GameActionLua:getAthleticsFirst4Rank(atker, 0x8) then    --第一次杀入竞技场10强
                cond = 7;
                itemId = 25;
                itemCount = 1;
                _GameActionLua:setAthleticsFirst4Rank(atker, 0x8);
            else
                cond = 8;
            end

        elseif(101 > _GameActionLua:getAthleticsRank(atker)) and row == 1 and (0 == _GameActionLua:getAthleticsFirst4Rank(atker, 0x10)) then --第一次杀入竞技场100强
            cond = 9;
            itemId = 24;
            itemCount = 2;
            _GameActionLua:setAthleticsFirst4Rank(atker, 0x10);
        elseif(201 > _GameActionLua:getAthleticsRank(atker)) and row == 1 and (0 == _GameActionLua:getAthleticsFirst4Rank(atker, 0x20)) then --第一次杀入竞技场200强
            cond = 10;
            itemId = 24;
            itemCount = 1;
            _GameActionLua:setAthleticsFirst4Rank(atker, 0x20);
        elseif(301 > _GameActionLua:getAthleticsRank(atker)) and row == 1 and (0 == _GameActionLua:getAthleticsFirst4Rank(atker, 0x40)) then --第一次杀入竞技场300强
            cond = 11;
            itemId = 23;
            itemCount = 1;
            _GameActionLua:setAthleticsFirst4Rank(atker, 0x40);
        elseif _GameActionLua:getAthleticsWinStreak(atker) == 3 then    --3连胜
            cond = 18;
            itemId = 22;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsWinStreak(atker) == 5 then    --5连胜
            cond = 17;
            itemId = 23;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsWinStreak(atker) == 10 then   --10连胜
            cond = 19;
            itemId = 23;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsWinStreak(atker) == 20 then   --20连胜
            cond = 20;
            itemId = 23;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsWinStreak(defer) > 19 then   --终结了XXX的(20+)连胜
            cond = 16;
            itemId = 24;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsWinStreak(defer) > 9 then    --终结了XXX的(10~19)连胜
            cond = 15;
            itemId = 23;
            itemCount = 2;
        elseif _GameActionLua:getAthleticsWinStreak(defer) > 4 then    --终结了XXX的(5~9)连胜
            cond = 14;
            itemId = 23;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsWinStreak(defer) > 3 then    --终结了XXX的(3~4)连胜
            cond = 13;
            itemId = 22;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsFailStreak(atker) == 25 then    --百折不挠
            cond = 22;
            itemId = 23;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsRankUpADay(atker) > 99 and row == 1 and 0 == _GameActionLua:getAthleticsFirst4Rank(atker, 0x100) then --一天内提升100个排名
            cond = 23;
            _GameActionLua:setAthleticsExtraChallenge(atker, _GameActionLua:getAthleticsRank(atk)*0.6);
            _GameActionLua:setAthleticsFirst4Rank(atker, 0x100);
        elseif _GameActionLua:getAthleticsRankUpADay(atker) > 9 and 0 == _GameActionLua:getAthleticsFirst4Rank(atker, 0x200) then     --一天内提升10个排名
            cond = 24;
            itemId = 22;
            itemCount = 1;
            _GameActionLua:setAthleticsFirst4Rank(atker, 0x200);
        elseif _GameActionLua:getAthleticsRankUpADay(atker) > 49 and 0 == _GameActionLua:getAthleticsFirst4Rank(atker, 0x400) then    --一天内提升50个排名
            cond = 25;
            itemId = 23;
            itemCount = 1;
            _GameActionLua:setAthleticsFirst4Rank(atker, 0x400);
        elseif math.random(1,100) < 6 then    --意外之喜
            cond = 30;
            itemId = 22;
            itemCount = 1;
        end

        if cond ~= 0 then
            package:AddItem(itemId, itemCount, 1);
            _GameActionLua:AddAthleticsEvent(row, atker, defer, cond, itemCount, itemId);
            cond = 0;
        end

    else
        local package = defer:GetPackage();

        if row == 1 and _GameActionLua:getAthleticsExtraChallenge(atker) ~= 0 then --特殊挑战失败
            cond = 27;
            itemId = 23;
            itemCount = 1;
            _GameActionLua:setAthleticsExtraChallenge(atker, 0);
        elseif _GameActionLua:getAthleticsWinStreak(atker) > 19 then       --终结了XXX的(20+)连胜
            cond = 16;
            itemId = 24;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsWinStreak(atker) > 9 then    --终结了XXX的(10~19)连胜
            cond = 15;
            itemId = 23;
            itemCount = 2;
        elseif _GameActionLua:getAthleticsWinStreak(atker) > 4 then    --终结了XXX的(5~9)连胜
            cond = 14;
            itemId = 23;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsWinStreak(atker) > 3 then    --终结了XXX的(3~4)连胜
            cond = 13;
            itemId = 22;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsBeWinStreak(defer) == 20 then    -- 连续被20人挑战而不败
            cond = 21;
            itemId = 23;
            itemCount = 1;
        elseif math.random(1,100) < 6 then    --虽败尤荣 
            cond = 29;
            itemId = 22;
            itemCount = 1;
        end

        if cond ~= 0 then
            package:AddItem(itemId, itemCount, 1);
            _GameActionLua:AddAthleticsEvent(row, atker, defer, cond, itemCount, itemId);
            cond = 0;
        end

    end

end

