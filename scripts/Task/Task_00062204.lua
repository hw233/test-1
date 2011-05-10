--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00062204()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(62204) or task:HasCompletedTask(62204) or task:HasSubmitedTask(62204) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00062204()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(62204) or task:HasCompletedTask(62204) or task:HasSubmitedTask(62204) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00062204()
	if GetPlayer():GetTaskMgr():HasCompletedTask(62204) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00062204(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(62204) == npcId and Task_Accept_00062204 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 62204
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ä¾¦æŸ¥ç„šéª¨çªŸ";
	elseif task:GetTaskSubmitNpc(62204) == npcId then
		if Task_Submit_00062204() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62204
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ä¾¦æŸ¥ç„šéª¨çªŸ";
		elseif task:HasAcceptedTask(62204) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62204
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ä¾¦æŸ¥ç„šéª¨çªŸ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00062204_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ï¼Œä½ å¬è¯´è¿‡ç„šéª¨çªŸè¿™ä¸ªåœ°æ–¹å—ï¼Ÿå¤ªææ€–äº†ï¼Œæˆ‘ä»¬å¾ˆå¤šå…„å¼Ÿå»äº†å°±æ²¡å†å›æ¥ã€‚ä½ èƒ½å¸®æˆ‘å»ä¾¦æŸ¥ä¸€ä¸‹é‚£é‡Œå—ï¼Ÿ";
	action.m_ActionMsg = "è¿™ä¸ªæ²¡é—®é¢˜ï¼Œä½ ç­‰æˆ‘çš„æ¶ˆæ¯ã€‚";
	return action;
end

function Task_00062204_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ ä¸€å®šè¦å°å¿ƒé‚£é‡Œçš„æ€ªç‰©ã€‚\n";
	action.m_ActionMsg = "æ”¾å¿ƒï¼Œé‚£äº›å®¶ä¼™ä¸æ˜¯æˆ‘çš„å¯¹æ‰‹ã€‚";
	return action;
end

function Task_00062204_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘ååˆ†ä½©æœä½ çš„å‹‡æ°”ï¼";
	action.m_ActionMsg = "";
	return action;
end

local Task_00062204_step_table = {
		[1] = Task_00062204_step_01,
		[2] = Task_00062204_step_02,
		[10] = Task_00062204_step_10,
		};

function Task_00062204_step(step)
	if Task_00062204_step_table[step] ~= nil then
		return Task_00062204_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00062204_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00062204() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(62204) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00062204_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(62204) then
		return false;
	end


	player:AddExp(10000);
	player:getCoin(20000);
	player:getTael(200);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00062204_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(62204);
end
