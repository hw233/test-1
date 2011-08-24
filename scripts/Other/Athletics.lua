--22 绿色斗剑场宝箱
--23 蓝色斗剑场宝箱
--24 紫色斗剑场宝箱
--25 橙色斗剑场宝箱

function RunAthleticsEvent(row, atker, defer, win)
    if 0 ~= _GameActionLua:getAthleticsFirst4Rank(atker, 0x80) then    --第一次竞技场挑战
        local package = atker:GetPackage();
        package:AddItem(22, 1, 1);
        _GameActionLua:AddAthleticsEvent(row, atker, defer, 10, 1, 22);
        _GameActionLua:setAthleticsFirst4Rank(atker, 0x80);
        return;
    end

    local cond = 0;
    local color = 0;
    local value = 0;
    local itemId = 0;
    local itemCount = 0;
    if win == 2 then
        if row == 1 and _GameActionLua:getAthleticsExtraChallenge(atker) ~= 0 then --放弃特殊挑战
            _GameActionLua:setAthleticsExtraChallenge(atker, 0);
        end

    elseif win == 1 then
        local package = atker:GetPackage();

        if row == 1 and 0 ~= _GameActionLua:getAthleticsFirst4Rank(atker, 0x100) then --特殊挑战胜利 一天连升200个排名
            cond = 16;
            color = 5;
            value = _GameActionLua:getAthleticsExtraChallenge(atker);
            itemId = 25;
            itemCount = 3;
            _GameActionLua:setAthleticsExtraChallenge(atker, 0);
        elseif row == 1 and 0 ~= _GameActionLua:getAthleticsFirst4Rank(atker, 0x200) then --特殊挑战胜利 一天连升100个排名
            cond = 16;
            color = 4;
            value = _GameActionLua:getAthleticsExtraChallenge(atker);
            itemId = 25;
            itemCount = 1;
            _GameActionLua:setAthleticsExtraChallenge(atker, 0);
        elseif row == 1 and 0 ~= _GameActionLua:getAthleticsFirst4Rank(atker, 0x400) then --特殊挑战胜利 一天连升50个排名
            cond = 16;
            color = 3;
            value = _GameActionLua:getAthleticsExtraChallenge(atker);
            itemId = 24;
            itemCount = 1;
            _GameActionLua:setAthleticsExtraChallenge(atker, 0);
        elseif row == 1 and 0 ~= _GameActionLua:getAthleticsFirst4Rank(atker, 0x800) then --特殊挑战胜利 一天连升20个排名
            cond = 16;
            color = 2;
            value = _GameActionLua:getAthleticsExtraChallenge(atker);
            itemId = 23;
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

        elseif (101 > _GameActionLua:getAthleticsRank(atker)) and row == 1 and (0 == _GameActionLua:getAthleticsFirst4Rank(atker, 0x10)) then --第一次杀入竞技场100强
            cond = 9;
            itemId = 24;
            value = 100;
            itemCount = 2;
            _GameActionLua:setAthleticsFirst4Rank(atker, 0x10);
        elseif (201 > _GameActionLua:getAthleticsRank(atker)) and row == 1 and (0 == _GameActionLua:getAthleticsFirst4Rank(atker, 0x20)) then --第一次杀入竞技场200强
            cond = 9;
            value = 200;
            itemId = 24;
            itemCount = 1;
            _GameActionLua:setAthleticsFirst4Rank(atker, 0x20);
        elseif (301 > _GameActionLua:getAthleticsRank(atker)) and row == 1 and (0 == _GameActionLua:getAthleticsFirst4Rank(atker, 0x40)) then --第一次杀入竞技场300强
            cond = 9;
            value = 300;
            itemId = 23;
            itemCount = 1;
            _GameActionLua:setAthleticsFirst4Rank(atker, 0x40);
        elseif _GameActionLua:getAthleticsWinStreak(atker) == 5 then    --5连胜
            cond = 12;
            value = _GameActionLua:getAthleticsWinStreak(atker);
            itemId = 22;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsWinStreak(atker) == 10 then    --10连胜
            cond = 12;
            value = _GameActionLua:getAthleticsWinStreak(atker);
            itemId = 23;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsWinStreak(atker) == 20 then   --20连胜
            cond = 12;
            value = _GameActionLua:getAthleticsWinStreak(atker);
            itemId = 24;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsWinStreak(atker) == 50 then   --50连胜
            cond = 12;
            value = _GameActionLua:getAthleticsWinStreak(atker);
            itemId = 25;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsWinStreak(defer) > 19 then   --终结了XXX的(20+)连胜
            cond = 11;
            value = _GameActionLua:getAthleticsWinStreak(defer);
            itemId = 24;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsWinStreak(defer) > 9 then    --终结了XXX的(10~19)连胜
            cond = 11;
            value = _GameActionLua:getAthleticsWinStreak(defer);
            itemId = 23;
            itemCount = 2;
        elseif _GameActionLua:getAthleticsWinStreak(defer) > 4 then    --终结了XXX的(5~9)连胜
            cond = 11;
            value = _GameActionLua:getAthleticsWinStreak(defer);
            itemId = 23;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsWinStreak(defer) > 3 then    --终结了XXX的(3~4)连胜
            cond = 11;
            value = _GameActionLua:getAthleticsWinStreak(defer);
            itemId = 22;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsFailStreak(atker) == 25 then    --百折不挠
            cond = 14;
            itemId = 23;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsRankUpADay(atker) > 199 and row == 1 and 0 == _GameActionLua:getAthleticsFirst4Rank(atker, 0x100) then --一天内提升200个排名
            cond = 15;
            color = 5;
            value = _GameActionLua:getAthleticsRankUpADay(atker);
            itemId = 25;
            itemCount = 1;
            _GameActionLua:setAthleticsExtraChallenge(atker, _GameActionLua:getAthleticsRank(atk)*0.5);
            _GameActionLua:setAthleticsFirst4Rank(atker, 0x100);
        elseif _GameActionLua:getAthleticsRankUpADay(atker) > 99 and row == 1 and 0 == _GameActionLua:getAthleticsFirst4Rank(atker, 0x200) then --一天内提升100个排名
            cond = 15;
            color = 4;
            value = _GameActionLua:getAthleticsRankUpADay(atker);
            itemId = 24;
            itemCount = 1;
            _GameActionLua:setAthleticsExtraChallenge(atker, _GameActionLua:getAthleticsRank(atk)*0.7);
            _GameActionLua:setAthleticsFirst4Rank(atker, 0x200);
        elseif _GameActionLua:getAthleticsRankUpADay(atker) > 49 and 0 == _GameActionLua:getAthleticsFirst4Rank(atker, 0x400) then     --一天内提升50个排名
            cond = 15;
            color = 3;
            value = _GameActionLua:getAthleticsRankUpADay(atker);
            itemId = 23;
            itemCount = 1;
            _GameActionLua:setAthleticsExtraChallenge(atker, _GameActionLua:getAthleticsRank(atk)*0.9);
            _GameActionLua:setAthleticsFirst4Rank(atker, 0x400);
        elseif _GameActionLua:getAthleticsRankUpADay(atker) > 19 and 0 == _GameActionLua:getAthleticsFirst4Rank(atker, 0x800) then    --一天内提升20个排名
            cond = 15;
            color = 2;
            value = _GameActionLua:getAthleticsRankUpADay(atker);
            itemId = 22;
            itemCount = 1;
            _GameActionLua:setAthleticsExtraChallenge(atker, _GameActionLua:getAthleticsRank(atk)*0.95);
            _GameActionLua:setAthleticsFirst4Rank(atker, 0x400);
        elseif math.random(1,100) < 8 then    --意外之喜
            cond = 17;
            itemId = 22;
            itemCount = 1;
        end

        if cond ~= 0 then
            package:AddItem(itemId, itemCount, 1);
            _GameActionLua:AddAthleticsEvent(row, atker, defer, (cond + color * 256 + value * 65536), itemCount, itemId);
            cond = 0;
        end

    else
        local package = defer:GetPackage();

        if row == 1 and _GameActionLua:getAthleticsExtraChallenge(atker) ~= 0 then --特殊挑战失败
            _GameActionLua:setAthleticsExtraChallenge(atker, 0);
        elseif _GameActionLua:getAthleticsWinStreak(atker) > 19 then       --终结了XXX的(20+)连胜
            cond = 11;
            value = _GameActionLua:getAthleticsWinStreak(atker);
            itemId = 24;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsWinStreak(atker) > 9 then    --终结了XXX的(10~19)连胜
            cond = 11;
            value = _GameActionLua:getAthleticsWinStreak(atker);
            itemId = 23;
            itemCount = 2;
        elseif _GameActionLua:getAthleticsWinStreak(atker) > 4 then    --终结了XXX的(5~9)连胜
            cond = 11;
            value = _GameActionLua:getAthleticsWinStreak(atker);
            itemId = 23;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsWinStreak(atker) > 3 then    --终结了XXX的(3~4)连胜
            cond = 13;
            value = _GameActionLua:getAthleticsWinStreak(atker);
            itemId = 22;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsBeWinStreak(defer) == 10 then    -- 连续被10人挑战而不败
            cond = 13;
            value = 10
            itemId = 22;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsBeWinStreak(defer) == 20 then    -- 连续被20人挑战而不败
            cond = 13;
            value = 20;
            itemId = 23;
            itemCount = 1;
        elseif _GameActionLua:getAthleticsBeWinStreak(defer) == 100 then    -- 连续被100人挑战而不败
            cond = 13;
            value = 100;
            value = 20;
            itemId = 24;
            itemCount = 1;
        end

        if cond ~= 0 then
            package:AddItem(itemId, itemCount, 1);
            _GameActionLua:AddAthleticsEvent(row, atker, defer, (cond + color * 256 + value * 65536), itemCount, itemId);
            cond = 0;
        end

    end

end

