--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000038()
	local player = GetPlayer();
	if player:GetLev() < 11 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(38) or task:HasCompletedTask(38) or task:HasSubmitedTask(38) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(36) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(36) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(36) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000038()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 11 then
		return false;
	end
	if task:HasAcceptedTask(38) or task:HasCompletedTask(38) or task:HasSubmitedTask(38) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(36) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(36) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(36) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000038()
	if GetPlayer():GetTaskMgr():HasCompletedTask(38) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000038(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(38) == npcId and Task_Accept_00000038 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 38
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç°åœºç½ªè¯";
	elseif task:GetTaskSubmitNpc(38) == npcId then
		if Task_Submit_00000038() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 38
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç°åœºç½ªè¯";
		elseif task:HasAcceptedTask(38) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 38
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç°åœºç½ªè¯";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000038_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¼ äº®è¿™ä¸ªæ¶äººå¾—å·²ä¼æ³•ï¼Œæœ¬å®˜è¿˜å¾—å¤šè°¢å°‘ä¾ å¸®åŠ©ï¼Œä¸è¿‡æœ¬å®˜å…¬åŠ¡ç¹å¿™ï¼ŒæŠ½ä¸å¼€èº«ï¼Œè¿˜è¯·å°‘ä¾ å¸¦ç€æˆ‘æ‰‹ä¸‹å‡ ä¸ªæ•å¿«å»ç‹è€ä¸ˆå®¶æœé›†è¿™ä¸ªæ¶å¾’çš„çŠ¯ç½ªè¯æ®ï¼Œä»¥ä¾›æ¥æ—¥å®¡è®¯ä¹‹ç”¨ã€‚";
	action.m_ActionMsg = "ä¸¾æ‰‹ä¹‹åŠ³ï¼ŒçŸ¥åºœå¤§äººæ‚¨å¤ªå®¢æ°”äº†ã€‚";
	return action;
end

function Task_00000038_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ æ‚¨è¾›è‹¦äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000038_step_table = {
		[1] = Task_00000038_step_01,
		[10] = Task_00000038_step_10,
		};

function Task_00000038_step(step)
	if Task_00000038_step_table[step] ~= nil then
		return Task_00000038_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000038_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000038() then
		return false;
	end
	if not task:AcceptTask(38) then
		return false;
	end
	task:AddTaskStep(38);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000038_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(38) then
		return false;
	end


	player:AddExp(4400);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000038_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(38);
end
