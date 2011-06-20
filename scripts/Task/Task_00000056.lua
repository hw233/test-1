--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000056()
	local player = GetPlayer();
	if player:GetLev() < 35 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(56) or task:HasCompletedTask(56) or task:HasSubmitedTask(56) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(55) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(55) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000056()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 35 then
		return false;
	end
	if task:HasAcceptedTask(56) or task:HasCompletedTask(56) or task:HasSubmitedTask(56) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(55) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(55) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000056()
	if GetPlayer():GetTaskMgr():HasCompletedTask(56) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000056(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(56) == npcId and Task_Accept_00000056 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 56
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "è¾Ÿæ¯’è¯è‰";
	elseif task:GetTaskSubmitNpc(56) == npcId then
		if Task_Submit_00000056() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 56
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "è¾Ÿæ¯’è¯è‰";
		elseif task:HasAcceptedTask(56) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 56
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "è¾Ÿæ¯’è¯è‰";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000056_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä¹Œé£è‰ç”Ÿé•¿åœ¨ç¦ä»™æ½­åº•å¾—ä¸€å—æ¯’çŸ³ä¹‹ä¸Šï¼Œé‚£æ¯’çŸ³å‘¨å›´åä¸ˆä»¥å†…ï¼Œå‘å‡ºä¸€ç§é»‘æ°›æ¯’é›¾ï¼Œéå¸¸å‰å®³ã€‚éœ€è¦ç˜´ç– çŸ³è¾¹çš„èŒç´ è‰æ‰å¯è¾Ÿæ¯’ï¼Œä½ ä»¬å»é‡‡ä¸€ç‚¹å›æ¥ï¼Œä¸è¿‡åƒä¸‡è¦å°å¿ƒçŸ³ç•”çš„æ¯’è›™ã€‚";
	action.m_ActionMsg = "å§¥å§¥æ”¾å¿ƒï¼Œæˆ‘ä¼šå°å¿ƒçš„ã€‚";
	return action;
end

function Task_00000056_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æœ‰äº†è¿™äº›è‰è¾Ÿæ¯’ï¼Œå–ä¹Œé£è‰å°±å¤šå‡ åˆ†æŠŠæ¡äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000056_step_table = {
		[1] = Task_00000056_step_01,
		[10] = Task_00000056_step_10,
		};

function Task_00000056_step(step)
	if Task_00000056_step_table[step] ~= nil then
		return Task_00000056_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000056_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000056() then
		return false;
	end
	if not task:AcceptTask(56) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000056_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(56) then
		return false;
	end


	player:AddExp(11111);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000056_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(56);
end
