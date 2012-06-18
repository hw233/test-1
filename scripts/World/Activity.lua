-- initializer

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

is_4_21 = false 
function is4_21()
    return is_4_21
end

is_6_22 = false 
function is6_22()
    return is_6_22
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

  if tm >= actTime0 and tm < actTime00 then
      setIsNewServer(true);
  else
      setIsNewServer(false);
  end

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
      setRechargeActive(true, 1)
  else
      setRechargeActive(false, 1)
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

  if tm >= actTime21 and tm < actTime22 then
      setWhiteLoveDay(true)
  else
      setWhiteLoveDay(false)
  end

  if tm >= actTime23 and tm < actTime24 then
      setTrumpEnchRet(true)
  else
      setTrumpEnchRet(false)
  end

  if tm >= actTime25 and tm < actTime26 then
      setRechargeActive(true, 2)
  else
      setRechargeActive(false, 2)
  end

  if tm >= actTime27 and tm < actTime28 then
      setFoolsDay(true)
  else
      setFoolsDay(false)
  end

  if tm >= actTime29 and tm < actTime30 then
      setChingMing(true)
  else
      setChingMing(false)
  end

  if tm >= actTime23 and tm < actTime30 then
      setCarnival(true)
  else
      setCarnival(false)
  end

  if tm >= actTime31 and tm < actTime32 then
      is_4_21 = true
  else
      is_4_21 = false
  end

  if tm >= actTime49 and tm < actTime50 then
      setJune(true)
  else
      setJune(false)
  end

  if tm >= actTime51 and tm < actTime52 then
      setJune1(true)
  else
      setJune1(false)
  end

  if tm >= actTime59 and tm < actTime60 then
      is_6_22 = true
      setDuanWu(true)
  else
      is_6_22 = false
      setDuanWu(false)
  end

  if isFBVersion() then
      if tm >= actTime101 and tm < actTime102 then
          setFighter1368(true)
      else
          setFighter1368(false)
      end

      if tm >= actTime103 and tm < actTime104 then
          setEnchantAct(true)
      else
          setEnchantAct(false)
      end

      if tm >= actTime105 and tm < actTime106 then
          setTrainFighter(true)
      else
          setTrainFighter(false)
      end

      if tm >= actTime107 and tm < actTime108 then
          setGemMergeAct(true)
      else
          setGemMergeAct(false)
      end

      if tm >= actTime109 and tm < actTime110 then
          setNetValentineDay(true)
      else
          setNetValentineDay(false)
      end

      if tm >= actTime111 and tm < actTime112 then
          setRechargeActive(true, 4)
      else
          setRechargeActive(false, 4)
      end

      setShuoShuo(false);
  else
      if tm >= actTime33 and tm < actTime34 then
          setMayDay(true)
      else
          setMayDay(false)
      end

      if tm >= actTime35 and tm < actTime36 then
          setMayDay1(true)
      else
          setMayDay1(false)
      end

      if tm >= actTime37 and tm < actTime38 then
          setYDMDAct(true)
      else
          setYDMDAct(false)
      end

      if tm >= actTime39 and tm < actTime40 then
          setSSDTAct(true)
      else
          setSSDTAct(false)
      end

      if tm >= actTime41 and tm < actTime42 then
          setBlueDiamondAct(true)
      else
          setBlueDiamondAct(false)
      end

      if tm >= actTime43 and tm < actTime44 then
          setYellowDiamondAct(true)
      else
          setYellowDiamondAct(false)
      end

      if tm >= actTime45 and tm < actTime46 then
          setQQGameAct(true)
      else
          setQQGameAct(false)
      end

      if tm >= actTime47 and tm < actTime48 then
          setRechargeActive(true, 4)
      else
          setRechargeActive(false, 4)
      end

      if tm >= actTime53 and tm < actTime54 then
          setEnchantGt11(true)
      else
          setEnchantGt11(false)
      end

      if tm >= actTime55 and tm < actTime56 then
          setRechargeActive(true, 8)
      else
          setRechargeActive(false, 8)
      end

      if tm >= actTime57 and tm < actTime58 then
          setRechargeNextRet(true)
      else
          setRechargeNextRet(false)
      end

      if tm >= actTime61 and tm < actTime62 then
          setEnchantAct(true)
      else
          setEnchantAct(false)
      end

      if tm >= actTime113 and tm < actTime114 then
          setMergeAthAct(true)
      else
          setMergeAthAct(false)
      end

      if tm >= actTime115 and tm < actTime116 then
          setFourCopAct(true)
      else
          setFourCopAct(false)
      end

      setShuoShuo(true);
  end

  setRC7Day(true)
  setCFriend(true);

  -- XXX: cancel auto heal
  setAutoHeal(true)
  loadStore();
end

function initActTime(y, m, d)
  local  SerStartTm = { ['year'] = y, ['month'] = m, ['day'] = d, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 }; --开服时间
  local  SerStartTm1 = { ['year'] = 2011, ['month'] = 10, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm2 = { ['year'] = 2011, ['month'] = 10, ['day'] = 29, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm3 = { ['year'] = 2011, ['month'] = 11, ['day'] = 11, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm4 = { ['year'] = 2011, ['month'] = 11, ['day'] = 23, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm5 = { ['year'] = 2011, ['month'] = 12, ['day'] = 23, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm6 = { ['year'] = 2012, ['month'] = 1, ['day'] = 12, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm7 = { ['year'] = 2012, ['month'] = 1, ['day'] = 16, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm8 = { ['year'] = 2012, ['month'] = 1, ['day'] = 17, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm9 = { ['year'] = 2012, ['month'] = 2, ['day'] = 11, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm10= { ['year'] = 2012, ['month'] = 3, ['day'] = 7, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm11= { ['year'] = 2012, ['month'] = 3, ['day'] = 12, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 装备法宝强化返利（3/19-3/25）
  local  SerStartTm12= { ['year'] = 2012, ['month'] = 3, ['day'] = 19, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 充值返利（3/23-3/31）
  local  SerStartTm13= { ['year'] = 2012, ['month'] = 3, ['day'] = 23, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 愚人节活动（3/29-4/1）
  local  SerStartTm14= { ['year'] = 2012, ['month'] = 3, ['day'] = 29, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 清明节
  local  SerStartTm15= { ['year'] = 2012, ['month'] = 4, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm16= { ['year'] = 2012, ['month'] = 4, ['day'] = 21, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };

  local  SerStartTm17= { ['year'] = 2012, ['month'] = 4, ['day'] = 28, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 搜搜地图 （5/14-5/20）
  local  SerStartTm18= { ['year'] = 2011, ['month'] = 5, ['day'] = 14, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };

  -- 蓝钻特权
  local  SerStartTm19= { ['year'] = 2012, ['month'] = 6, ['day'] = 12, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 黄钻特权
  local  SerStartTm20= { ['year'] = 2012, ['month'] = 6, ['day'] = 12, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 游戏大厅特权
  local  SerStartTm21= { ['year'] = 2012, ['month'] = 6, ['day'] = 11, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };

  -- 充值返利（5/16-5/27）
  local  SerStartTm22= { ['year'] = 2012, ['month'] = 5, ['day'] = 16, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };

  -- 儿童节 (5/28-6/3)(5/28-6/30)
  local  SerStartTm23= { ['year'] = 2012, ['month'] = 5, ['day'] = 28, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 限时强化
  local  SerStartTm24= { ['year'] = 2012, ['month'] = 6, ['day'] = 6, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 充值后期返利(过段时间反利)
  local  SerStartTm25= { ['year'] = 2012, ['month'] = 6, ['day'] = 18, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 6.22限时购买
  local  SerStartTm26= { ['year'] = 2012, ['month'] = 6, ['day'] = 22, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm27= { ['year'] = 2012, ['month'] = 6, ['day'] = 25, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };

  local  SerStartTm101 = { ['year'] = 2012, ['month'] = 4, ['day'] = 25, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm102 = { ['year'] = 2012, ['month'] = 6, ['day'] = 8, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm103 = { ['year'] = 2012, ['month'] = 6, ['day'] = 16, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm104 = { ['year'] = 2012, ['month'] = 5, ['day'] = 19, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- FB充值返利（5/23-5/27）
  local  SerStartTm105= { ['year'] = 2012, ['month'] = 5, ['day'] = 16, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 合服斗剑排名奖励增加至30名活动(6/25-7/1)
  local  SerStartTm106 = { ['year'] = 2012, ['month'] = 6, ['day'] = 25, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 四大名捕（6/22-7/21）
  local  SerStartTm107 = { ['year'] = 2012, ['month'] = 6, ['day'] = 22, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };

  actTime0 = os.time(SerStartTm);
  actTime00 = os.time(SerStartTm) + 7 * 86400;

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

  actTime21 = os.time(SerStartTm11);
  actTime22 = os.time(SerStartTm11) + 7 * 86400;

  actTime23 = os.time(SerStartTm12);
  actTime24 = os.time(SerStartTm12) + 7 * 86400;

  actTime25 = os.time(SerStartTm13);
  actTime26 = os.time(SerStartTm13) + 9 * 86400;

  actTime27 = os.time(SerStartTm14);
  actTime28 = os.time(SerStartTm14) + 4 * 86400;

  actTime29 = os.time(SerStartTm15);
  actTime30 = os.time(SerStartTm15) + 8 * 86400;

  actTime31 = os.time(SerStartTm16);
  actTime32 = os.time(SerStartTm16) + 11 * 86400;

  actTime33 = os.time(SerStartTm17);
  actTime34 = os.time(SerStartTm17) + 7 * 86400;

  actTime35 = os.time(SerStartTm17);
  actTime36 = os.time(SerStartTm17) + 34 * 86400;
  
  actTime37 = os.time(SerStartTm17);
  actTime38 = os.time(SerStartTm17) + 10 * 86400;

  actTime39 = os.time(SerStartTm18);
  actTime40 = os.time(SerStartTm18) + 7 * 86400;

  actTime41 = os.time(SerStartTm19);
  actTime42 = os.time(SerStartTm19) + 19 * 86400;

  actTime43 = os.time(SerStartTm20);
  actTime44 = os.time(SerStartTm20) + 19 * 86400;

  actTime45 = os.time(SerStartTm21);
  actTime46 = os.time(SerStartTm21) + 14 * 86400;

  actTime47 = os.time(SerStartTm22);
  actTime48 = os.time(SerStartTm22) + 12 * 86400;

  actTime49 = os.time(SerStartTm23);
  actTime50 = os.time(SerStartTm23) + 7 * 86400;

  actTime51 = os.time(SerStartTm23);
  actTime52 = os.time(SerStartTm23) + 34 * 86400;

  actTime53 = os.time(SerStartTm24);
  actTime54 = os.time(SerStartTm24) + 12 * 86400;

  actTime55 = os.time(SerStartTm24);
  actTime56 = os.time(SerStartTm24) + 12 * 86400;

  actTime57 = os.time(SerStartTm25);
  actTime58 = os.time(SerStartTm25) + 13 * 86400;
  setRechargeNextRetStart(actTime57, actTime58)

  actTime59 = os.time(SerStartTm26);
  actTime60 = os.time(SerStartTm26) + 9 * 86400;

  actTime61 = os.time(SerStartTm27);
  actTime62 = os.time(SerStartTm27) + 6 * 86400;

  actTime101 = os.time(SerStartTm101);
  actTime102 = os.time(SerStartTm101) + 8 * 86400;

  actTime103 = os.time(SerStartTm102);
  actTime104 = os.time(SerStartTm102) + 8 * 86400;

  actTime105 = os.time(SerStartTm102);
  actTime106 = os.time(SerStartTm102) + 8 * 86400;

  actTime107 = os.time(SerStartTm103);
  actTime108 = os.time(SerStartTm103) + 7 * 86400;

  actTime109 = os.time(SerStartTm104);
  actTime110 = os.time(SerStartTm104) + 7 * 86400;

  actTime111 = os.time(SerStartTm105);
  actTime112 = os.time(SerStartTm105) + 12 * 86400;

  actTime113 = os.time(SerStartTm106);
  actTime114 = os.time(SerStartTm106) + 7 * 86400;
  actTime115 = os.time(SerStartTm107);
  actTime116 = os.time(SerStartTm107) + 30 * 86400;

  onActivityCheck(os.time() + 30);

  loadTitle();
  loadRareAnimals()
  loadTeamCopyAwardsCfg()
end

function onAthleticsNewBox(t, c)
  return c
end

