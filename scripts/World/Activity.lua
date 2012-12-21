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

is_6_23 = false 
function is6_23()
    return is_6_23
end

is_6_25 = false 
function is6_25()
    return is_6_25
end

is_7_1 = false 
function is7_1()
    return is_7_1
end

is_7_10_15 = false
function is7_10_15()
    return is_7_10_15
end

is_7_13_16= false
function is7_13_16()
    return is_7_13_16
end

is_7_14_15 = false
function is7_14_15()
    return is_7_14_15
end

is_7_16_8_15 = false
function is7_16_8_15()
    return is_7_16_8_15 
end

is_7_13 = false
function is7_13()
    return is_7_13
end

is_6_29 = false 
function is6_29()
    return is_6_29
end

is_0810_0901 = false
function is0810_0901()
    return is_0810_0901
end

is_0808_0814 = false
function is0808_0814()
    return is_0808_0814
end

is_0921_0924 = false
function is0921_0924()
    return is_0921_0924
end

is_0926_0927 = false
function is0926_0927()
    return is_0926_0927
end

is_1116_1122 = false
function is1116_1122()
    return is_1116_1122
end

is_1221_1227 = false
function is1221_1227()
    return is_1221_1227
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

  if tm >= actTime203 and tm < actTime204 then
      setQixi(true)
  else
      setQixi(false)
  end

  if tm >= actTime218 and tm < actTime219 then
      setWansheng(true)
  else
      setWansheng(false)
  end

  if tm >= actTime205 and tm < actTime206 then
      setGuoqing(true)
  else
      setGuoqing(false)
  end

  if tm >= actTime207 and tm < actTime207_1 then
      setTownReward_10_15(true)
  else
      setTownReward_10_15(false)
  end

  if tm >= actTime210 and tm < actTime211 then
      setConsume918(true)
  else
      setConsume918(false)
  end

  if tm >= actTime212 and tm < actTime213 then
      setLoginAward(true)
  else
      setLoginAward(false)
  end

  if tm >= actTime214 and tm < actTime215 then
      set3366PrivilegeAct(true)
  else
      set3366PrivilegeAct(false)
  end
  
  if tm >= actTime216 and tm < actTime217 then
      setBDSuperman(true)
  else
      setBDSuperman(false)
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

      if tm >= actTime200 and tm < actTime201 then
          is_6_29 = true
          setICAct(true)
      else
          is_6_29 = false 
          setICAct(false)
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

      if tm >= actTime51 and tm < actTime52 then
          setJune1(true)
      else
          setJune1(false)
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
          is_6_25 = true
          setEnchantAct(true)
      else
          is_6_25 = false
          setEnchantAct(false)
      end

      if tm >= actTime63 and tm < actTime64 then
          is_6_23 = true
      else
          is_6_23 = false
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

      if tm >= actTime65 and tm < actTime66 then
          setRechargeActive(true, 16)
      elseif tm >= actTime73 and tm < actTime74 then
          setRechargeActive(true, 16)
      elseif tm >= actTime83 and tm < actTime84 then
          setRechargeActive(true, 16)
      else
          setRechargeActive(false, 16)
      end

      if tm >= actTime67 and tm < actTime68 then
          setJuly(true)
      else
          setJuly(false)
      end

      if tm >= actTime69 and tm < actTime70 then
          is_7_1 = true
      else
          is_7_1 = false 
      end

      if tm >= actTime71 and tm < actTime72 then
          setEnchantAct(true)
      else
          setEnchantAct(false)
      end

      if tm >= actTime75 and tm < actTime76 then
          is_7_10_15 = true
      else
          is_7_10_15 = false 
      end

      if tm >= actTime77 and tm < actTime78 then
          setTrumpEnchRet(true)
          is_7_13_16 = true
          setPExpItems(true)
      else
          setTrumpEnchRet(false)
          is_7_13_16 = false
          setPExpItems(false)
      end

      if tm >= actTime79 and tm < actTime80 then
          is_7_14_15 = true
      else
          is_7_14_15 = false
      end

      if tm >= actTime81 and tm < actTime82 then
          is_7_16_8_15 = true
      else
          is_7_16_8_15 = false 
      end

      if tm >= actTime83 and tm < actTime84 then
          setSoSoMapBegin(actTime83)
      end

      if tm >= actTime85 and tm < actTime86 then
          setOpenTest(true)
          setRechargeActive(true, 16)
          setConsumeActive(true)
          setSoSoMapBegin(actTime85)
      else
          setOpenTest(false)
          setRechargeActive(false, 16)
          setConsumeActive(false)
      end

      if tm >= actTime87 and tm < actTime88 then
          setConsumeActive(true)
      else
          setConsumeActive(false)
      end

      if tm >= actTime89 and tm < actTime90 then
          setQQVipAct(true)
      else
          setQQVipAct(false)
      end

      if tm >= actTime91 and tm < actTime92 then
          is_0810_0901 = true
      else
          is_0810_0901 = false
      end

      if tm >= actTime93 and tm < actTime94 then
          setFallAct(true)
          is_0808_0814 = true
      else
          setFallAct(false)
          is_0808_0814 = false
      end

      if tm >= actTime95 and tm < actTime96 then
          setRechargeActive(true, 16)
      else
          setRechargeActive(false, 16)
      end

      if tm >= actTime97 and tm < actTime98 then
          setRechargeActive3366(true, 16)
      else
          setRechargeActive3366(false, 16)
      end

      if tm >= actTime99 and tm < actTime100 then
          setRechargeActive(true, 16)
      else
          setRechargeActive(false, 16)
      end

      if tm >= actTime500 and tm < actTime500_1 then
          setConsumeActive(true)
      else
          setConsumeActive(false)
      end

      if tm >= actTime501 and tm < actTime501_1 then
          setRechargeActive3366(true, 16)
      else
          setRechargeActive3366(false, 16)
      end

      if tm >= actTime502 and tm < actTime502_1 then
          setRechargeActive(true, 16)
          setNeedRechargeRank(true)
      else
          setRechargeActive(false, 16)
          setNeedRechargeRank(false)
      end

      if tm >= actTime506 and tm < actTime506_1 then
          setYearActive(true)
      else
          setYearActive(false)
      end

      if tm >= actTime507 and tm < actTime507_1 then
          setQgameGiftAct(true)
      else
          setQgameGiftAct(false)
      end

      if tm >= actTime504 and tm < actTime504_1 then
          setQgameGiftAct(true)
      else
          setQgameGiftAct(false)
      end

      if tm >= actTime505 and tm < actTime505_1 then
          setKillMonsterAct(true)
      else
          setKillMonsterAct(false)
      end
      if tm >= actTime503 and tm < actTime503_1 then
          setRechargeActive(true, 16)
          setNeedRechargeRank(true)
      else
          setRechargeActive(false, 16)
          setNeedRechargeRank(false)
      end

      if tm >= actTime508 and tm < actTime508_1 then
          setRechargeActive(true, 16)
      else
          setRechargeActive(false, 16)
      end

      if tm >= actTime509 and tm < actTime509_1 then
          is_0921_0924 = true
      else
          is_0921_0924 = false
      end

      if tm >= actTime510 and tm < actTime510_1 then
          is_0926_0927 = true
      else
          is_0926_0927 = false
      end
      
      if tm >= actTime511 and tm < actTime511_1 then
          setRechargeActive(true, 16)
          setNeedRechargeRank(false)
      elseif tm >= actTime512 and tm < actTime512_1 then
          setRechargeActive(true, 16)
          setNeedRechargeRank(true)
      else
          setRechargeActive(false, 16)
          setNeedRechargeRank(false)
      end

      if tm >= actTime513 and tm < actTime513_1 then
          setConsumeActive(true)
      else
          setConsumeActive(false)
      end

      if tm >= actTime514 and tm < actTime514_1 then
          setRechargeActive(true, 16)
      else
          setRechargeActive(false, 16)
      end

      if tm >= actTime515 and tm < actTime515_1 then
          setNeedRechargeRank(true)
          setRechargeActive(true, 16)
      else
          setRechargeActive(false, 16)
          setNeedRechargeRank(false)
      end

      if tm >= actTime516 and tm < actTime516_1 then
          setRechargeActive(true, 16)
      else
          setRechargeActive(false, 16)
      end

      if tm >= actTime517 and tm < actTime517_1 then
          setConsumeActive(true)
          setNeedConsumeRank(true)
      else
          setConsumeActive(false)
          setNeedConsumeRank(false)
      end

      if tm >= actTime518 and tm < actTime518_1 then
          setRechargeActive(true, 16)
      else
          setRechargeActive(false, 16)
      end

      if tm >= actTime519 and tm < actTime519_1 then
          setRechargeActive(true, 16)
          setNeedRechargeRank(true)
      else
          setRechargeActive(false, 16)
          setNeedRechargeRank(false)
      end

      if tm >= actTime520 and tm < actTime520_1 then
          setTgcEvent(true)
      else
          setTgcEvent(false)
      end

      if tm >= actTime521 and tm < actTime521_1 then
          setRechargeActive(true, 16)
      elseif tm >= actTime522 and tm < actTime522_1 then
          setRechargeActive(true, 16)
          setNeedRechargeRank(true)
      else
          setRechargeActive(false, 16)
          setNeedRechargeRank(false)
      end

      if tm >= actTime525 and tm < actTime525_1 then
          setTgcEvent(true)
      else
          setTgcEvent(false)
      end

      if tm >= actTime523 and tm < actTime523_1 then
          setRechargeActive(true, 16)
      elseif tm >= actTime527 and tm < actTime527_1 then
          setRechargeActive(true, 16)
          setNeedRechargeRank(true)
      elseif tm >= actTime528 and tm < actTime528_1 then
          setRechargeActive(true, 16)
          setNeedRechargeRank(false)
      elseif tm >= actTime529 and tm < actTime529_1 then
          setRechargeActive(true, 16)
          setNeedRechargeRank(true)
      elseif tm >= actTime530 and tm < actTime530_1 then
          setRechargeActive(true, 16)
          setNeedRechargeRank(true)
      elseif tm >= actTime531 and tm < actTime531_1 then
          setRechargeActive(true, 16)
          setNeedRechargeRank(false)
      elseif tm >= actTime533 and tm < actTime533_1 then
          setRechargeActive(true, 16)
          setNeedRechargeRank(false)
      else
          setRechargeActive(false, 16)
          setNeedRechargeRank(false)
      end

      if tm >= actTime532 and tm < actTime532_1 then
          is_1116_1122 = true
      else
          is_1116_1122 = false
      end
      
      if tm >= actTime220 and tm < actTime221 then
          setQzonePYPrivilegeAct(true)
      else
          setQzonePYPrivilegeAct(false)
      end

      if tm >= actTime526 and tm < actTime526_1 then
          setConsumeActive(true)
          setNeedConsumeRank(true)
      else
          setConsumeActive(false)
          setNeedConsumeRank(false)
      end

      if tm >= actTime534 and tm < actTime534_1 then
          setRechargeActive(true, 16)
          setNeedRechargeRank(true)
      else
          setRechargeActive(false, 16)
          setNeedRechargeRank(false)
      end

      --if tm >= actTime535 and tm < actTime535_1 then
          setGoodVoiceAct(true)
          setTgcEvent(true)
      --else
      --    setGoodVoiceAct(false)
      --    setTgcEvent(false)
      --end

      -- if tm >= actTime536 and tm < actTime536_1 then
          set3366GiftAct(true)
      -- else
      --    set3366GiftAct(false)
      -- end

      if tm >= actTime537 and tm < actTime537_1 then
          setConsumeActive(true)
      else
          setConsumeActive(false)
      end

      if tm >= actTime220_0 and tm < actTime220_1 then
          setTowerLoginAct( (tm - actTime220_0) / 86400 + 1)
      else
          setTowerLoginAct(0)
      end

      if tm >= actTime221_0 and tm < actTime221_1 then
          setFeastLoginAct(true)
          is_1221_1227 = true
      else
          setFeastLoginAct(false)
          is_1221_1227 = false
      end

      setShuoShuo(true);
  end

  if tm >= actTime222 and tm < actTime223 then
      set11Act(true)
  else
      set11Act(false)
  end
  if tm >= actTime218_0 and tm < actTime218_1 then
      set9215Act(true)
  else
      set9215Act(false)
  end
  if tm >= actTime219_0 and tm < actTime219_1 then
      setSSToolbarAct(true)
  else
      setSSToolbarAct(false)
  end
  if tm >= actTime220_0 and tm < actTime220_1 then
      setSnowAct(true)
  else
      setSnowAct(false)
  end

  setRC7Day(true)
  setCFriend(true);

  -- XXX: cancel auto heal
  setAutoHeal(true)
  setCopyFrontWinSwitch(true)
  loadStore();
end

function initActTime(y, m, d)
  local  SerStartTm = { ['year'] = y, ['month'] = m, ['day'] = d, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 }; --开服时间
  local  SerStartTm1 = { ['year'] = 2011, ['month'] = 10, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm2 = { ['year'] = 2011, ['month'] = 10, ['day'] = 29, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm3 = { ['year'] = 2011, ['month'] = 11, ['day'] = 11, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm4 = { ['year'] = 2012, ['month'] = 12, ['day'] = 4, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
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
  local  SerStartTm19= { ['year'] = 2012, ['month'] = 8, ['day'] = 10, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 黄钻特权
  local  SerStartTm20= { ['year'] = 2012, ['month'] = 8, ['day'] = 24, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 游戏大厅特权
  local  SerStartTm21= { ['year'] = 2012, ['month'] = 9, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };

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
  local  SerStartTm27= { ['year'] = 2012, ['month'] = 6, ['day'] = 23, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm28= { ['year'] = 2012, ['month'] = 6, ['day'] = 25, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm29= { ['year'] = 2012, ['month'] = 7, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm30= { ['year'] = 2012, ['month'] = 7, ['day'] = 7, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm31= { ['year'] = 2012, ['month'] = 7, ['day'] = 9, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm32= { ['year'] = 2012, ['month'] = 7, ['day'] = 10, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm33= { ['year'] = 2012, ['month'] = 7, ['day'] = 13, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm34= { ['year'] = 2012, ['month'] = 7, ['day'] = 14, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm35= { ['year'] = 2012, ['month'] = 7, ['day'] = 16, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm36= { ['year'] = 2012, ['month'] = 7, ['day'] = 17, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm37= { ['year'] = 2012, ['month'] = 7, ['day'] = 25, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm38= { ['year'] = 2012, ['month'] = 8, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- vip会员
  local  SerStartTm39= { ['year'] = 2012, ['month'] = 8, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 坐骑6销售时间
  local  SerStartTm40= { ['year'] = 2012, ['month'] = 8, ['day'] = 10, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 掉落活动
  local  SerStartTm41= { ['year'] = 2012, ['month'] = 8, ['day'] = 8, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 充值返利（8/14-8/16）
  local  SerStartTm42= { ['year'] = 2012, ['month'] = 8, ['day'] = 14, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 3366合作充值活动（8/17-8/19）
  local  SerStartTm43= { ['year'] = 2012, ['month'] = 8, ['day'] = 17, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 七夕充值活动
  local  SerStartTm44= { ['year'] = 2012, ['month'] = 8, ['day'] = 21, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 消耗活动
  local SerStartTm500 = { ['year'] = 2012, ['month'] = 8, ['day'] = 28, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 3366合作充值活动（8/30-8/31）
  local  SerStartTm501= { ['year'] = 2012, ['month'] = 8, ['day'] = 30, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm502= { ['year'] = 2012, ['month'] = 9, ['day'] = 6, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 斩妖
  local  SerStartTm505= { ['year'] = 2012, ['month'] = 9, ['day'] = 16, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 周岁红包送不停
  local  SerStartTm506= { ['year'] = 2012, ['month'] = 9, ['day'] = 13, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- Qgame大厅礼包
  local  SerStartTm507= { ['year'] = 2012, ['month'] = 9, ['day'] = 17, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm504= { ['year'] = 2012, ['month'] = 11, ['day'] = 11, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };

  local  SerStartTm503= { ['year'] = 2012, ['month'] = 9, ['day'] = 13, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  --充值返礼(9/22-9/23)
  local  SerStartTm508= { ['year'] = 2012, ['month'] = 9, ['day'] = 22, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  --变身法宝(11/14-11/18)
  local  SerStartTm509= { ['year'] = 2012, ['month'] = 11, ['day'] = 14, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  --商城限购(9/26-9/27)
  local  SerStartTm510= { ['year'] = 2012, ['month'] = 9, ['day'] = 26, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm511= { ['year'] = 2012, ['month'] = 9, ['day'] = 27, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm512= { ['year'] = 2012, ['month'] = 10, ['day'] = 5, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm513= { ['year'] = 2012, ['month'] = 10, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm514= { ['year'] = 2012, ['month'] = 10, ['day'] = 12, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm515= { ['year'] = 2012, ['month'] = 10, ['day'] = 16, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm516= { ['year'] = 2012, ['month'] = 10, ['day'] = 19, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm517= { ['year'] = 2012, ['month'] = 10, ['day'] = 19, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm518= { ['year'] = 2012, ['month'] = 10, ['day'] = 23, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm519= { ['year'] = 2012, ['month'] = 10, ['day'] = 26, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm520= { ['year'] = 2012, ['month'] = 10, ['day'] = 31, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm521= { ['year'] = 2012, ['month'] = 10, ['day'] = 30, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm522= { ['year'] = 2012, ['month'] = 11, ['day'] = 3, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm523= { ['year'] = 2012, ['month'] = 11, ['day'] = 6, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };

  local  SerStartTm525= { ['year'] = 2012, ['month'] = 12, ['day'] = 4, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm526= { ['year'] = 2012, ['month'] = 11, ['day'] = 23, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm527= { ['year'] = 2012, ['month'] = 11, ['day'] = 27, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm528= { ['year'] = 2012, ['month'] = 11, ['day'] = 23, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm529= { ['year'] = 2012, ['month'] = 11, ['day'] = 16, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm530= { ['year'] = 2012, ['month'] = 11, ['day'] = 10, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm531= { ['year'] = 2012, ['month'] = 11, ['day'] = 13, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm532= { ['year'] = 2012, ['month'] = 11, ['day'] = 16, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm533= { ['year'] = 2012, ['month'] = 12, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm534= { ['year'] = 2012, ['month'] = 12, ['day'] = 8, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 蜀山好声音合作
  local  SerStartTm535= { ['year'] = 2012, ['month'] = 12, ['day'] = 14, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 3366合作礼包
  local  SerStartTm536= { ['year'] = 2012, ['month'] = 12, ['day'] = 11, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm537= { ['year'] = 2012, ['month'] = 12, ['day'] = 15, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };

  -- 繁体版
  local  SerStartTm101 = { ['year'] = 2012, ['month'] = 4, ['day'] = 25, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm102 = { ['year'] = 2012, ['month'] = 6, ['day'] = 8, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm103 = { ['year'] = 2012, ['month'] = 6, ['day'] = 16, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm104 = { ['year'] = 2012, ['month'] = 5, ['day'] = 19, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- FB充值返利（5/23-5/27）
  local  SerStartTm105= { ['year'] = 2012, ['month'] = 5, ['day'] = 16, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 合服斗剑排名奖励增加至30名活动(6/25-7/1)
  local  SerStartTm106 = { ['year'] = 2012, ['month'] = 6, ['day'] = 25, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 四大名捕（6/25-7/31）
  -- 单身剑侠party(与四大名捕相同)
  local  SerStartTm107 = { ['year'] = 2012, ['month'] = 11, ['day'] = 9, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm200= { ['year'] = 2012, ['month'] = 6, ['day'] = 29, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm201= { ['year'] = 2012, ['month'] = 7, ['day'] = 11, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };

  -- 七夕
  local  SerStartTm202= { ['year'] = 2012, ['month'] = 8, ['day'] = 21, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  --商城消费大酬宾(9/18-9/24)
  local  SerStartTm210= { ['year'] = 2012, ['month'] = 9, ['day'] = 18, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 中秋国庆
  local  SerStartTm205= { ['year'] = 2012, ['month'] = 10, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm206= { ['year'] = 2012, ['month'] = 10, ['day'] = 8, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  --镇妖封神 修仙夺宝
  local  SerStartTm207= { ['year'] = 2012, ['month'] = 10, ['day'] = 15, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm207_1= { ['year'] = 2012, ['month'] = 10, ['day'] = 19, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };

  -- 2012.10.14登录抽奖合作
  local SerStartTm211 = { ['year'] = 2012, ['month'] = 11, ['day'] = 8, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };

  -- 3366特权活动(通游戏大厅)
  local  SerStartTm212 = { ['year'] = 2012, ['month'] = 10, ['day'] = 16, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 蓝钻优惠活动(蓝钻超人) //10.19上线推迟
  local SerStartTm213 = { ['year'] = 2012, ['month'] = 10, ['day'] = 16, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local SerStartTm214 = { ['year'] = 2012, ['month'] = 10, ['day'] = 16, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 万圣节
  local  SerStartTm215 = { ['year'] = 2012, ['month'] = 11, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 空间、朋友平台特权活动
  local  SerStartTm216 = { ['year'] = 2012, ['month'] = 11, ['day'] = 9, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };

  -- 光棍节活动
  local  SerStartTm217 = { ['year'] = 2012, ['month'] = 11, ['day'] = 9, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 逍遥嘉年华礼包
  local  SerStartTm218 = { ['year'] = 2012, ['month'] = 11, ['day'] = 16, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  --搜搜工具栏活动
  local  SerStartTm219 = { ['year'] = 2012, ['month'] = 11, ['day'] = 25, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm219_1 = { ['year'] = 2012, ['month'] = 12, ['day'] = 26, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  --圣诞堆雪人活动
  local  SerStartTm220 = { ['year'] = 2012, ['month'] = 12, ['day'] = 22, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTm220_1 = { ['year'] = 2012, ['month'] = 12, ['day'] = 29, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };


  -- 楼一登录奖励时间段（21~27)
  local  SerStartTm220 = { ['year'] = 2012, ['month'] = 12, ['day'] = 21, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  -- 节日套装人人拿(22~28)
  local  SerStartTm221 = { ['year'] = 2012, ['month'] = 12, ['day'] = 22, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };


  --300-399越南版用了
  actTime0 = os.time(SerStartTm);
  actTime00 = os.time(SerStartTm) + 7 * 86400;

  actTime1 = os.time(SerStartTm1);
  actTime2 = os.time(SerStartTm1) + 7 * 86400;

  actTime3 = os.time(SerStartTm2);
  actTime4 = os.time(SerStartTm2) + 4 * 86400;

  actTime5 = os.time(SerStartTm3);
  actTime6 = os.time(SerStartTm3) + 3 * 86400;
  
  actTime7 = os.time(SerStartTm4);
  actTime8 = os.time(SerStartTm4) + 12 * 86400;

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
  actTime42 = os.time(SerStartTm19) + 14 * 86400;

  actTime43 = os.time(SerStartTm20);
  actTime44 = os.time(SerStartTm20) + 17 * 86400;

  actTime45 = os.time(SerStartTm21);
  actTime46 = os.time(SerStartTm21) + 61 * 86400;

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

  actTime61 = os.time(SerStartTm28);
  actTime62 = os.time(SerStartTm28) + 6 * 86400;

  actTime63 = os.time(SerStartTm27);
  actTime64 = os.time(SerStartTm27) + 8 * 86400;

  actTime65 = os.time(SerStartTm29);
  actTime66 = os.time(SerStartTm29) + 6 * 86400;

  actTime67 = os.time(SerStartTm29);
  actTime68 = os.time(SerStartTm29) + 31 * 86400;

  actTime69 = os.time(SerStartTm29);
  actTime70 = os.time(SerStartTm29) + 15 * 86400;

  actTime71 = os.time(SerStartTm30);
  actTime72 = os.time(SerStartTm30) + 2 * 86400;

  actTime73 = os.time(SerStartTm31);
  actTime74 = os.time(SerStartTm31) + 5 * 86400;

  actTime75 = os.time(SerStartTm32);
  actTime76 = os.time(SerStartTm32) + 6 * 86400;

  actTime77 = os.time(SerStartTm33);
  actTime78 = os.time(SerStartTm33) + 4 * 86400;

  actTime79 = os.time(SerStartTm34);
  actTime80 = os.time(SerStartTm34) + 2 * 86400;

  actTime81 = os.time(SerStartTm35);
  actTime82 = os.time(SerStartTm35) + 31 * 86400;

  actTime83 = os.time(SerStartTm36);
  actTime84 = os.time(SerStartTm36) + 7 * 86400;

  actTime85 = os.time(SerStartTm37);
  actTime86 = os.time(SerStartTm37) + 7 * 86400;

  actTime87 = os.time(SerStartTm38);
  actTime88 = os.time(SerStartTm38) + 7 * 86400;

  actTime89 = os.time(SerStartTm39);
  actTime90 = os.time(SerStartTm39) + 3 * 86400;

  actTime91= os.time(SerStartTm40);
  actTime92= os.time(SerStartTm40) + 23 * 86400;

  actTime93= os.time(SerStartTm41);
  actTime94= os.time(SerStartTm41) + 7 * 86400;

  actTime95 = os.time(SerStartTm42);
  actTime96 = os.time(SerStartTm42) + 3 * 86400;

  actTime97 = os.time(SerStartTm43);
  actTime98 = os.time(SerStartTm43) + 3 * 86400;

  actTime99 = os.time(SerStartTm44);
  actTime100 = os.time(SerStartTm44) + 7 * 86400;

  actTime500    = os.time(SerStartTm500);
  actTime500_1  = os.time(SerStartTm500) + 7 * 86400;

  actTime501 = os.time(SerStartTm501);
  actTime501_1 = os.time(SerStartTm501) + 2 * 86400;

  actTime502 = os.time(SerStartTm502);
  actTime502_1 = os.time(SerStartTm502) + 4 * 86400;

  actTime505 = os.time(SerStartTm505);
  actTime505_1 = os.time(SerStartTm505) + 8 * 86400;

  actTime506 = os.time(SerStartTm506);
  actTime506_1 = os.time(SerStartTm506) + 18 * 86400;

  actTime507 = os.time(SerStartTm507);
  actTime507_1 = os.time(SerStartTm507) + 3 * 86400;

  actTime504 = os.time(SerStartTm504);
  actTime504_1 = os.time(SerStartTm504) + 7 * 86400;

  actTime503 = os.time(SerStartTm503);
  actTime503_1 = os.time(SerStartTm503) + 4 * 86400;

  actTime508 = os.time(SerStartTm508);
  actTime508_1 = os.time(SerStartTm508) + 2 * 86400;

  actTime509 = os.time(SerStartTm509);
  actTime509_1 = os.time(SerStartTm509) + 5 * 86400;

  actTime510 = os.time(SerStartTm510);
  actTime510_1 = os.time(SerStartTm510) + 2 * 86400;

  actTime511 = os.time(SerStartTm511);
  actTime511_1 = os.time(SerStartTm511) + 4 * 86400;

  actTime512 = os.time(SerStartTm512);
  actTime512_1 = os.time(SerStartTm512) + 2 * 86400;

  actTime513 = os.time(SerStartTm513);
  actTime513_1 = os.time(SerStartTm513) + 7 * 86400;

  actTime514 = os.time(SerStartTm514);
  actTime514_1 = os.time(SerStartTm514) + 3 * 86400;

  actTime515 = os.time(SerStartTm515);
  actTime515_1 = os.time(SerStartTm515) + 3 * 86400;

  actTime516 = os.time(SerStartTm516);
  actTime516_1 = os.time(SerStartTm516) + 4 * 86400;

  actTime517 = os.time(SerStartTm517);
  actTime517_1 = os.time(SerStartTm517) + 4 * 86400;

  actTime518 = os.time(SerStartTm518);
  actTime518_1 = os.time(SerStartTm518) + 3 * 86400;

  actTime519 = os.time(SerStartTm519);
  actTime519_1 = os.time(SerStartTm519) + 3 * 86400;

  actTime520 = os.time(SerStartTm520);
  actTime520_1 = os.time(SerStartTm520) + 26 * 86400;

  actTime521 = os.time(SerStartTm521);
  actTime521_1 = os.time(SerStartTm521) + 4 * 86400;

  actTime522 = os.time(SerStartTm522);
  actTime522_1 = os.time(SerStartTm522) + 3 * 86400;

  actTime523 = os.time(SerStartTm523);
  actTime523_1 = os.time(SerStartTm523) + 4 * 86400;

  actTime525 = os.time(SerStartTm525);
  actTime525_1 = os.time(SerStartTm525) + 26 * 86400;

  actTime526 = os.time(SerStartTm526);
  actTime526_1 = os.time(SerStartTm526) + 4 * 86400;

  actTime527 = os.time(SerStartTm527);
  actTime527_1 = os.time(SerStartTm527) + 4 * 86400;

  actTime528 = os.time(SerStartTm528);
  actTime528_1 = os.time(SerStartTm528) + 4 * 86400;

  actTime529 = os.time(SerStartTm529);
  actTime529_1 = os.time(SerStartTm529) + 7 * 86400;

  actTime530 = os.time(SerStartTm530);
  actTime530_1 = os.time(SerStartTm530) + 3 * 86400;

  actTime531 = os.time(SerStartTm531);
  actTime531_1 = os.time(SerStartTm531) + 3 * 86400;

  actTime532 = os.time(SerStartTm532);
  actTime532_1 = os.time(SerStartTm532) + 7 * 86400;

  actTime533 = os.time(SerStartTm533);
  actTime533_1 = os.time(SerStartTm533) + 5 * 86400;

  actTime534 = os.time(SerStartTm534);
  actTime534_1 = os.time(SerStartTm534) + 20 * 86400;

  actTime535 = os.time(SerStartTm535);
  actTime535_1 = os.time(SerStartTm535) + 4 * 86400;

  actTime536 = os.time(SerStartTm536);
  actTime536_1 = os.time(SerStartTm536) + 1 * 86400;

  actTime537 = os.time(SerStartTm537);
  actTime537_1 = os.time(SerStartTm537) + 7 * 86400;

  -- 繁体版
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
  actTime116 = os.time(SerStartTm107) + 7 * 86400;
  actTime200= os.time(SerStartTm200);
  actTime201= os.time(SerStartTm200) + 6 * 86400;

  actTime202= os.time(SerStartTm201);
  setLevelAwardEnd(actTime202)

  actTime203= os.time(SerStartTm202);
  actTime204= os.time(SerStartTm202) + 7 * 86400;

  actTime205= os.time(SerStartTm205);
  actTime206= os.time(SerStartTm206);

  actTime207= os.time(SerStartTm207);
  actTime207_1= os.time(SerStartTm207_1);

  actTime210= os.time(SerStartTm210);
  actTime211= os.time(SerStartTm210) + 7 * 86400;

  actTime212= os.time(SerStartTm211);
  actTime213= os.time(SerStartTm211) + 3 * 86400;

  actTime214= os.time(SerStartTm212);
  actTime215= os.time(SerStartTm212) + 1 * 86400;
  
  actTime216= os.time(SerStartTm213);
  actTime217= os.time(SerStartTm214);

  actTime218= os.time(SerStartTm215);
  actTime219= os.time(SerStartTm215) + 7 * 86400;

  actTime220= os.time(SerStartTm216);
  actTime221= os.time(SerStartTm216) + 38 * 86400;

  actTime222= os.time(SerStartTm217);
  actTime223= os.time(SerStartTm217) + 7 * 86400;

  actTime218_0= os.time(SerStartTm218);
  actTime218_1= os.time(SerStartTm218) + 7 * 86400;

  actTime219_0= os.time(SerStartTm219);
  actTime219_1= os.time(SerStartTm219_1);

  actTime220_0= os.time(SerStartTm220);
<<<<<<< HEAD
  actTime220_1= os.time(SerStartTm220_1);

=======
  actTime220_1= os.time(SerStartTm220) + 7 * 86400;

  actTime221_0= os.time(SerStartTm221);
  actTime221_1= os.time(SerStartTm221) + 7 * 86400;
>>>>>>> 5b5bf27056402f10798d93b31dfe33bac604b52c

  onActivityCheck(os.time() + 30);

  loadTitle();
  loadRareAnimals()
  loadTeamCopyAwardsCfg()
end

function onAthleticsNewBox(t, c)
  return c
end

