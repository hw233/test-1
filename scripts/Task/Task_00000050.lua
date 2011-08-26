--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000050()
	local player = GetPlayer();
	if player:GetLev() < 32 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50) or task:HasCompletedTask(50) or task:HasSubmitedTask(50) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000050()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 32 then
		return false;
	end
	if task:HasAcceptedTask(50) or task:HasCompletedTask(50) or task:HasSubmitedTask(50) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000050()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000050(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50) == npcId and Task_Accept_00000050 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å†³æˆ˜ä¹‹åœ°";
	elseif task:GetTaskSubmitNpc(50) == npcId then
		if Task_Submit_00000050() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å†³æˆ˜ä¹‹åœ°";
		elseif task:HasAcceptedTask(50) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å†³æˆ˜ä¹‹åœ°";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000050_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ–¹æ‰é†‰é“äººå‰è¾ˆå»äº†æ…ˆäº‘å¯ºï¼Œä¸ä¼—å¦–äººçº¦å®šåœ¨é­å®¶åœºå±•å¼€å†³æˆ˜ï¼Œå±Šæ—¶æˆéƒ½å‘¨é­çš„æ­£é“å‰‘ä»™éƒ½å›å»é­å®¶åœºï¼Œå°‘ä¾ ä½ æ­£å¥½å¯ä»¥å»åŠ©å¤§å®¶ä¸€è‡‚ä¹‹åŠ›ï¼Œå¤šè¯›æ€å‡ ä¸ªé‚ªæ´¾çš„å¦–äººã€‚";
	action.m_ActionMsg = "é™¤æ¶æ‰¬å–„ï¼Œæˆ‘è‡ªç„¶æ˜¯ä¹‰ä¸å®¹è¾ã€‚";
	return action;
end

function Task_00000050_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é­å®¶åœºä¸€æˆ˜ï¼Œé‚ªæ´¾å¦–äººè¢«æ‰“å¾—æ˜¯ä¸¢ç›”å¼ƒç”²å•Šï¼Œå“ˆå“ˆã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000050_step_table = {
		[1] = Task_00000050_step_01,
		[10] = Task_00000050_step_10,
		};

function Task_00000050_step(step)
	if Task_00000050_step_table[step] ~= nil then
		return Task_00000050_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000050_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000050() then
		return false;
	end
	if not task:AcceptTask(50) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000050_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(50) then
		return false;
	end


	player:AddExp(13000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000050_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(50);
end
