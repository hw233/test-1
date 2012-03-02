-- initializer

actTime1 = 0;
actTime2 = 0;
actTime3 = 0;
actTime4 = 0;

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
    setDungeonPrice(2, 50)
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

  if tm >= actTime3 and tm < actTime4 then
      setHalloween(true);
  else
      setHalloween(false);
  end

  if tm >= actTime5 and tm < actTime6 then
      setSingleDay(true);
  else
      setSingleDay(false);
  end

  if tm >= actTime7 and tm < actTime8 then
      setThanksgiving(true);
  else
      setThanksgiving(false);
  end

  if tm >= actTime9 and tm < actTime10 then
      setChristmas(true)
  else
      setChristmas(false)
  end

  if tm >= actTime11 and tm < actTime12 then
      setNewYear(true)
  else
      setNewYear(false)
  end

  if tm >= actTime13 and tm < actTime14 then
      setBlueactiveday(true)
  else
      setBlueactiveday(false)
  end

  if tm >= actTime15 and tm < actTime16 then
      setRechargeActive(true)
  else
      setRechargeActive(false)
  end

  if tm >= actTime17 and tm < actTime18 then
      setValentineDay(true)
  else
      setValentineDay(false)
  end

  if tm >= actTime19 and tm < actTime20 then
      setGirlDay(true)
  else
      setGirlDay(false)
  end

  loadStore()
end

function initActTime(y, m, d)
  local  SerStartTm1 = { ['year'] = 2011, ['month'] = 10, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm2 = { ['year'] = 2011, ['month'] = 10, ['day'] = 29, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm3 = { ['year'] = 2011, ['month'] = 11, ['day'] = 11, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm4 = { ['year'] = 2011, ['month'] = 11, ['day'] = 23, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm5 = { ['year'] = 2011, ['month'] = 12, ['day'] = 23, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm6 = { ['year'] = 2012, ['month'] = 1, ['day'] = 12, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm7 = { ['year'] = 2012, ['month'] = 1, ['day'] = 16, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm8 = { ['year'] = 2012, ['month'] = 1, ['day'] = 17, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm9 = { ['year'] = 2012, ['month'] = 2, ['day'] = 11, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  --local  SerStartTm10= { ['year'] = 2012, ['month'] = 3, ['day'] = 7, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm10= { ['year'] = 2012, ['month'] = 2, ['day'] = 29, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };

  actTime1 = os.time(SerStartTm1);
  actTime2 = os.time(SerStartTm1) + 7 * 86400;

  actTime3 = os.time(SerStartTm2);
  actTime4 = os.time(SerStartTm2) + 4 * 86400;

  actTime5 = os.time(SerStartTm3);
  actTime6 = os.time(SerStartTm3) + 3 * 86400;
  
  actTime7 = os.time(SerStartTm4);
  actTime8 = os.time(SerStartTm4) + 7 * 86400;

  actTime9 = os.time(SerStartTm5);
  actTime10 = os.time(SerStartTm5) + 12 * 86400;

  actTime11 = os.time(SerStartTm6);
  actTime12 = os.time(SerStartTm6) + 26 * 86400;

  actTime13 = os.time(SerStartTm7);
  actTime14 = os.time(SerStartTm7) + 22 * 86400;

  actTime15 = os.time(SerStartTm8);
  actTime16 = os.time(SerStartTm8) + 21 * 86400;

  actTime17 = os.time(SerStartTm9);
  actTime18 = os.time(SerStartTm9) + 7 * 86400;

  actTime19 = os.time(SerStartTm10);
  actTime20 = os.time(SerStartTm10) + 2 * 86400;

  onActivityCheck(os.time() + 30);

  loadStore();
  loadTitle();
  loadRareAnimals()
  loadTeamCopyAwardsCfg()
end

function onAthleticsNewBox(t, c)
  return c
end

