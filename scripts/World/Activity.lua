-- initializer

actTime1 = 0;
actTime2 = 0;

serverName = nil
serverNum = 0

oldServersMax = {
["kingxin"] = 32,
["4399"] = 3,
["huanlang"] = 2,
["kuwan8"] = 16,
["37wan"] = 8,
["7k7k"] = 13,
["game2"] = 24,
["56"] = 42,
["baofeng"] = 3,
["game5"] = 2,
["7wan"] = 35,
["runup"] = 25,
["wowan365"] = 14,
["niua"] = 7,
["duole"] = 7,
["gamebean"] = 21,
["16768"] = 2,
["txwy"] = 8,
["29ww"] = 9,
["nextworld"] = 8,
["quyouxi"] = 2,
["houdao"] = 1,
["51wan"] = 9,
["uu98"] = 2,
["pps"] = 1,
["kuwo"] = 2,
["sougames"] = 1,
["95k"] = 2,
["xman"] = 1,
["yesjd"] = 1,
["6711"] = 1
};

function setServer(n, num)
  serverName = n
  serverNum = num
end

function onActivityCheck(tm)
  
  local osmax = oldServersMax[serverName]
  if osmax ~= nil and serverNum <= osmax then
    local freeEnter = false
    if serverName == 'kingxin' then
      local freeStartTime = os.time({ ['year'] = 2011, ['month'] = 1, ['day'] = 20, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 });
      local freeEndTime = os.time({ ['year'] = 2011, ['month'] = 1, ['day'] = 26, ['hour'] = 23, ['min'] = 50, ['sec'] = 0 });
      setDungeonPrice(0, 0)
      if tm >= freeStartTime and tm < freeEndTime then
        freeEnter = true
      end
    end
    if freeEnter then
      setDungeonPrice(1, 0)
      setDungeonPrice(2, 0)
    else
      setDungeonPrice(1, 10)
      setDungeonPrice(2, 10)
    end
  else
    setDungeonPrice(0, 0)
    setDungeonPrice(1, 50)
    setDungeonPrice(2, 100)
  end
  setDungeonCount(1)
  setTavernInterval(1 * 3600)
  setTavernRate(100)
  
  setRecruitCost(20);
  setTavernBlueCount(25);
  setTavernPurpleCount(100);
  setTavernOrangeCount(400);


  if tm >= actTime1 and tm < actTime2 then
    setShiMenActiveCount(5);
    setYaMenActiveCount(5);
    setCopyActiveCount(2);
    setFrontMapActiveCount(2);
    setNationalDay(true);
  else
    setShiMenActiveCount(0);
    setYaMenActiveCount(0);
    setCopyActiveCount(0);
    setFrontMapActiveCount(0);
    setNationalDay(false);
  end

end

function initActTime(y, m, d)
  --local  SerStartTm = { ['year'] = 2011, ['month'] = 10, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };--活动时间
  local  SerStartTm = { ['year'] = 2011, ['month'] = 9, ['day'] = 24, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };--活动时间
  actTime1 = os.time(SerStartTm);
  actTime2 = os.time(SerStartTm) + 7 * 86400;
  
  onActivityCheck(os.time() + 30);

  loadStore();
  loadTitle();
end

function onAthleticsNewBox(t, c)
  return c
end

