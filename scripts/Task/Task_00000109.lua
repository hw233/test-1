--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000109()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(109) or task:HasCompletedTask(109) or task:HasSubmitedTask(109) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(108) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(108) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(108) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000109()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(109) or task:HasCompletedTask(109) or task:HasSubmitedTask(109) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(108) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(108) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(108) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000109()
	if GetPlayer():GetTaskMgr():HasCompletedTask(109) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000109(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(109) == npcId and Task_Accept_00000109 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 109
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "é‡‘è›ŠèŠ±";
	elseif task:GetTaskSubmitNpc(109) == npcId then
		if Task_Submit_00000109() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 109
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "é‡‘è›ŠèŠ±";
		elseif task:HasAcceptedTask(109) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 109
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "é‡‘è›ŠèŠ±";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000109_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è™½ç„¶å°†é‡‘èš•é™¤å»ï¼Œä¸è¿‡ç»¿è¢ç”¨æ¥é¥²å…»é‡‘èš•çš„é‡‘è›ŠèŠ±ä¹Ÿæ˜¯ç»æ¯’ä¹‹ç‰©ï¼Œå…¶ä¸­æœ‰ä¸å°‘è¢«ç»¿è¢ç”¨é‚ªæ³•ç‚¼åˆ¶æˆå‚€å„¡ï¼Œå‡¶æ¶å¼‚å¸¸ï¼Œ"..GetPlayerName(GetPlayer()).."ä½ å»å°†è¿™äº›é‡‘è›ŠèŠ±å‚€å„¡æ¶ˆç­ã€‚";
	action.m_ActionMsg = "è°¨éµæŒæ•™çœŸäººæ³•æ—¨ã€‚";
	return action;
end

function Task_00000109_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é‡‘èš•å’Œé‡‘è›ŠèŠ±è¢«å°½æ•°ç­é™¤ï¼Œç»¿è¢æƒ³å†ç‚¼é‡‘èš•è›Šææ€•æ˜¯æ²¡é‚£ä¹ˆå®¹æ˜“äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000109_step_table = {
		[1] = Task_00000109_step_01,
		[10] = Task_00000109_step_10,
		};

function Task_00000109_step(step)
	if Task_00000109_step_table[step] ~= nil then
		return Task_00000109_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000109_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000109() then
		return false;
	end
	if not task:AcceptTask(109) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000109_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(109) then
		return false;
	end


	player:AddExp(24000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000109_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(109);
end
