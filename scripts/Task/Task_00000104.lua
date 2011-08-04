--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000104()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(104) or task:HasCompletedTask(104) or task:HasSubmitedTask(104) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(103) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(103) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(103) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000104()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(104) or task:HasCompletedTask(104) or task:HasSubmitedTask(104) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(103) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(103) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(103) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000104()
	if GetPlayer():GetTaskMgr():HasCompletedTask(104) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000104(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(104) == npcId and Task_Accept_00000104 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 104
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å…µè§£";
	elseif task:GetTaskSubmitNpc(104) == npcId then
		if Task_Submit_00000104() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 104
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å…µè§£";
		elseif task:HasAcceptedTask(104) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 104
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å…µè§£";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000104_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç®—ç®—å¤©åŠ«å°†è‡³ï¼Œ"..GetPlayerName(GetPlayer()).."ä½ çœ‹å¤©è¾¹é‚£æœµçº¢äº‘å³æ˜¯æˆ‘å°†é­é€¢çš„åŠ«äº‘ã€‚å±Šæ—¶å¤©åŠ«ä¸€è‡³ï¼Œå°‘ä¾ ä½ å°±ç”¨è¿™é’ç‰›å‰‘åˆºæˆ‘è‚‰èº«åŠ©æˆ‘å…µè§£ï¼Œè¿™æ ·æˆ‘å…ƒå©´ä¹Ÿå¯ä»¥é‡æ–°è½¬ä¸–å†ä¿®åŠŸå¾·ã€‚";
	action.m_ActionMsg = "å¥½çš„ï¼Œé‚£æ•æ™šè¾ˆå†’çŠ¯äº†ã€‚";
	return action;
end

function Task_00000104_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¤šè°¢å°‘ä¾ ä¸ºæˆ‘è„±åŠ«ä¹‹äº‹å¥”æ³¢ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000104_step_table = {
		[1] = Task_00000104_step_01,
		[10] = Task_00000104_step_10,
		};

function Task_00000104_step(step)
	if Task_00000104_step_table[step] ~= nil then
		return Task_00000104_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000104_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000104() then
		return false;
	end
	if not task:AcceptTask(104) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000104_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(104) then
		return false;
	end


	player:AddExp(5664);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000104_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(104);
end
