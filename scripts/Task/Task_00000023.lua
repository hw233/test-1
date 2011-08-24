--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000023()
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(23) or task:HasCompletedTask(23) or task:HasSubmitedTask(23) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(22) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(22) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(22) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000023()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(23) or task:HasCompletedTask(23) or task:HasSubmitedTask(23) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(22) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(22) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(22) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000023()
	if GetPlayer():GetTaskMgr():HasCompletedTask(23) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000023(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(23) == npcId and Task_Accept_00000023 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 23
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "é£å°˜å¼‚äºº";
	elseif task:GetTaskSubmitNpc(23) == npcId then
		if Task_Submit_00000023() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 23
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "é£å°˜å¼‚äºº";
		elseif task:HasAcceptedTask(23) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 23
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "é£å°˜å¼‚äºº";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000023_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è°¢è°¢å°‘ä¾ å‡ºæ‰‹ç›¸æ•‘ï¼Œæ–¹æ‰ä½ ä¸é‚£æ¶åƒ§æ‰“æ–—æ—¶ï¼Œæœ‰ä¸€ä¸ªæ€ªè€å¤´ç»è¿‡ï¼Œè®©æˆ‘è½¬å‘Šè¯´ï¼Œå¾…ä¼šè®©ä½ å»ç»¿ç«¹æ—æ‰¾ä»–ï¼Œæ¥ç€å°±ä¸è§äº†ã€‚æˆ‘æƒ³ä»–ä¸€å®šæ˜¯ä¸ªé£å°˜å¼‚äººï¼Œå°‘ä¾ èµ¶å¿«å»æ‰¾ä»–å§ã€‚";
	action.m_ActionMsg = "æœ‰è¿™ç§äº‹æƒ…ï¼Œé‚£æˆ‘èµ°äº†ï¼Œè¯·å¤šä¿é‡ã€‚";
	return action;
end

function Task_00000023_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°æœ‹å‹å¿«è¿‡æ¥ï¼Œè®©æˆ‘æ¥é€—ä»–ä»¬ç©ç©ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000023_step_table = {
		[1] = Task_00000023_step_01,
		[10] = Task_00000023_step_10,
		};

function Task_00000023_step(step)
	if Task_00000023_step_table[step] ~= nil then
		return Task_00000023_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000023_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000023() then
		return false;
	end
	if not task:AcceptTask(23) then
		return false;
	end
	task:AddTaskStep(23);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000023_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(23) then
		return false;
	end


	player:AddExp(1800);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000023_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(23);
end
