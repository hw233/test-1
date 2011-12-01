--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000058()
	local player = GetPlayer();
	if player:GetLev() < 35 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(58) or task:HasCompletedTask(58) or task:HasSubmitedTask(58) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(57) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(57) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(57) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000058()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 35 then
		return false;
	end
	if task:HasAcceptedTask(58) or task:HasCompletedTask(58) or task:HasSubmitedTask(58) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(57) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(57) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(57) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000058()
	if GetPlayer():GetTaskMgr():HasCompletedTask(58) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000058(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(58) == npcId and Task_Accept_00000058 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 58
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "åªæ¬ ä¸œé£";
	elseif task:GetTaskSubmitNpc(58) == npcId then
		if Task_Submit_00000058() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 58
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "åªæ¬ ä¸œé£";
		elseif task:HasAcceptedTask(58) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 58
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "åªæ¬ ä¸œé£";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000058_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç°åœ¨ä¸‡äº‹å…·å¤‡ï¼Œåªæ¬ ä¸œé£äº†ï¼Œç°åœ¨æ˜¯æœ€åä¸€æ­¥ï¼Œè¿™é‡Œæ˜¯å°å¾’ç”³è‹¥å…°åœ¨é¤éœå¤§å¸ˆå¤„å€Ÿæ¥çš„ç´«çƒŸé”„ï¼Œå¯ä»¥åœ¨æ¯’çŸ³ä¸Šåˆ¨å‡ºä¹Œé£è‰æ¥ã€‚ä¸è¿‡è¿™å—æ¯’çŸ³æ•£å‘çš„æ¯’é›¾å¾ˆæ˜¯å‡¶æ¯’ï¼Œæ‰€ä»¥é‡‡é›†æ—¶åƒä¸‡è¦å°å¿ƒã€‚";
	action.m_ActionMsg = "æˆ‘ä¼šæ³¨æ„çš„ï¼Œå§¥å§¥æ‚¨æ”¾å¿ƒå§ã€‚";
	return action;
end

function Task_00000058_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™ä¹Œé£è‰å¯æ²»å„ç§é‚ªæ´¾è›Šæ¯’ï¼Œä½ è¦å°å¿ƒä½¿ç”¨ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000058_step_table = {
		[1] = Task_00000058_step_01,
		[10] = Task_00000058_step_10,
		};

function Task_00000058_step(step)
	if Task_00000058_step_table[step] ~= nil then
		return Task_00000058_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000058_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000058() then
		return false;
	end
	if not task:AcceptTask(58) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000058_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(58) then
		return false;
	end


	player:AddExp(30000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000058_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(58);
end
