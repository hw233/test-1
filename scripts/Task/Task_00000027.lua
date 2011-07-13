--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000027()
	local player = GetPlayer();
	if player:GetLev() < 22 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(27) or task:HasCompletedTask(27) or task:HasSubmitedTask(27) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(26) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(26) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(26) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000027()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 22 then
		return false;
	end
	if task:HasAcceptedTask(27) or task:HasCompletedTask(27) or task:HasSubmitedTask(27) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(26) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(26) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(26) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000027()
	if GetPlayer():GetTaskMgr():HasCompletedTask(27) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000027(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(27) == npcId and Task_Accept_00000027 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 27
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "åˆ‡ç£‹æ­¦è‰º";
	elseif task:GetTaskSubmitNpc(27) == npcId then
		if Task_Submit_00000027() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 27
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "åˆ‡ç£‹æ­¦è‰º";
		elseif task:HasAcceptedTask(27) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 27
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "åˆ‡ç£‹æ­¦è‰º";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000027_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ çœ‹èµ·æ¥ä¸æ€ä¹ˆæ ·å˜›ï¼Œåˆä¸é«˜åˆä¸å£®ï¼Œå¯æ˜¯å¸ˆçˆ¶è¯´ä½ æˆå°±ä¸å‡¡ï¼Œæ­¦åŠŸä¹Ÿä¸é”™ï¼Œæˆ‘ä»¬å¯ä¸æœæ°”å“¦ï¼Œè¦ä¸è¦æ¥è·Ÿæˆ‘ä»¬æ¯”è¯•ä¸€ä¸‹ã€‚";
	action.m_ActionMsg = "é‚£å°±æ­æ•¬ä¸å¦‚ä»å‘½äº†å“¦ã€‚";
	return action;
end

function Task_00000027_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ æœç„¶é«˜æ˜ï¼Œæˆ‘ä»¬ç®—æ˜¯å½»åº•çš„æœæ°”äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000027_step_table = {
		[1] = Task_00000027_step_01,
		[10] = Task_00000027_step_10,
		};

function Task_00000027_step(step)
	if Task_00000027_step_table[step] ~= nil then
		return Task_00000027_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000027_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000027() then
		return false;
	end
	if not task:AcceptTask(27) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000027_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(27) then
		return false;
	end


	player:AddExp(3333);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000027_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(27);
end
