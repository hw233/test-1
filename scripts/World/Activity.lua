function onActivityCheck(tm)

--  if tm >= actTime250 and tm < actTime250_1 then
--      --setCollectCardAct(true)
--  else
--      --setCollectCardAct(false)
--  end


  --setAutoHeal(true)
  --setHeroIslandAct(true)
  --setCopyFrontWinSwitch(true)
end

function initActTime(y, m, d)

  local  SerStartTmq1= { ['year'] = 2014, ['month'] = 9, ['day'] = 6, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartTmq2= { ['year'] = 2014, ['month'] = 7, ['day'] = 19, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };

  --众里寻他
  local  SerStartSeekingHer1= { ['year'] = 2014, ['month'] = 10, ['day'] = 20, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
  local  SerStartSeekingHer2= { ['year'] = 2014, ['month'] = 10, ['day'] = 27, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };

  local  SerStartTmq4= { ['year'] = 2014, ['month'] = 9, ['day'] = 16, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };


  --actTimeq4= os.time(SerStartTmq4)
  --actTimeq4_1= os.time(SerStartTmq4) + 10 * 86400;

  --onActivityCheck(os.time() + 30);
end

function onAthleticsNewBox(t, c)
  return c
end
