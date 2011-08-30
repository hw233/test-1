--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000122()
	local player = GetPlayer();
	if player:GetLev() < 46 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(122) or task:HasCompletedTask(122) or task:HasSubmitedTask(122) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000122()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 46 then
		return false;
	end
	if task:HasAcceptedTask(122) or task:HasCompletedTask(122) or task:HasSubmitedTask(122) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000122()
	if GetPlayer():GetTaskMgr():HasCompletedTask(122) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000122(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(122) == npcId and Task_Accept_00000122 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 122
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "é’èºå››é­”";
	elseif task:GetTaskSubmitNpc(122) == npcId then
		if Task_Submit_00000122() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 122
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "é’èºå››é­”";
		elseif task:HasAcceptedTask(122) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 122
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "é’èºå››é­”";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000122_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é’èºé­”å®«çš„å‡ ä¸ªé­”å¤´å¥´å½¹æˆ‘ä»¬å·²ä¹…ï¼Œå°¤å…¶æ˜¯é‚£å››é­”ä¼Šçº¢æ¨±å¸¸å¸¸ä¸‹å±±ä½œæ¶ï¼Œæ¯æ¬¡è·¯è¿‡æˆ‘ä»¬æ¸…è¿œå¯ºï¼Œå¯ºå†…åƒ§ä¼—è½»åˆ™è¢«æ‰“éª‚ï¼Œé‡åˆ™ä¸¢å¤±æ€§å‘½â€¦â€¦æˆ‘ä»¬æ˜¯æ•¢æ€’ä¸æ•¢è¨€ï¼Œå¸Œæœ›å°‘ä¾ å¯ä»¥ä¸ºæˆ‘ä»¬é™¤æ‰è¿™ä¸ªæ¶äººã€‚";
	action.m_ActionMsg = "ä½ æ”¾å¿ƒï¼Œæˆ‘ä¸€å®šå¸®ä½ æŠ¥è¿™ä¸ªä»‡ã€‚";
	return action;
end

function Task_00000122_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æƒ³ä¸åˆ°ç»ˆäºå¾—æŠ¥æ­¤ä»‡ï¼ŒçœŸæ˜¯è°¢è°¢"..GetPlayerName(GetPlayer()).."ä½ äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000122_step_table = {
		[1] = Task_00000122_step_01,
		[10] = Task_00000122_step_10,
		};

function Task_00000122_step(step)
	if Task_00000122_step_table[step] ~= nil then
		return Task_00000122_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000122_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000122() then
		return false;
	end
	if not task:AcceptTask(122) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000122_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(122) then
		return false;
	end


	player:AddExp(35000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000122_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(122);
end
