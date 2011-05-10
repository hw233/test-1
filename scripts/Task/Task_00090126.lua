--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090126()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90126) or task:HasCompletedTask(90126) or task:HasSubmitedTask(90126) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90125) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90125) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090126()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90126) or task:HasCompletedTask(90126) or task:HasSubmitedTask(90126) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90125) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90125) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090126()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90126) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090126(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90126) == npcId and Task_Accept_00090126 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90126
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¤œæ‘©çš„æˆé•¿";
	elseif task:GetTaskSubmitNpc(90126) == npcId then
		if Task_Submit_00090126() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90126
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¤œæ‘©çš„æˆé•¿";
		elseif task:HasAcceptedTask(90126) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90126
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¤œæ‘©çš„æˆé•¿";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090126_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "å‹‡å£«ï¼Œä½ å»ä¾¦æŸ¥ä¸‹æ¯æ¾é•‡ï¼Œæˆ‘ä»¬æ€äº†è¿™ä¹ˆå¤šç‹¼äººï¼Œæ¯æœ¨è¶è‚¯å®šä¼šæœ‰ä¸‹ä¸€æ­¥è¡ŒåŠ¨ã€‚";
	action.m_ActionMsg = "é‚£äº›æ€ªç‰©å·²ç„¶ä¸æˆæ°”å€™ã€‚";
	return action;
end

function Task_00090126_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿˜æ˜¯ä¸èƒ½å¤ªè½»æ•Œï¼Œç°åœ¨å¤œæ‘©åŠ¿åŠ›ä¸æ—¥ä¿±å¢ï¼ŒåŒç›Ÿè¶Šæ¥è¶Šå¤šï¼Œä¸èƒ½å°è§†ã€‚";
	action.m_ActionMsg = "é‚£å¥½ï¼Œæˆ‘è¿™å°±å»çœ‹çœ‹ã€‚";
	return action;
end

function Task_00090126_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æœç„¶ä¸å‡ºæˆ‘æ‰€æ–™ã€‚	";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090126_step_table = {
		[1] = Task_00090126_step_01,
		[2] = Task_00090126_step_02,
		[10] = Task_00090126_step_10,
		};

function Task_00090126_step(step)
	if Task_00090126_step_table[step] ~= nil then
		return Task_00090126_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090126_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090126() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90126) then
		return false;
	end
	task:AddTaskStep(90126);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090126_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90126) then
		return false;
	end


	player:AddExp(725000);
	player:getCoin(318000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090126_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90126);
end
