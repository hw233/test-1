--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00030507()
	local player = GetPlayer();
	if player:GetLev() < 42 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30507) or task:HasCompletedTask(30507) or task:HasSubmitedTask(30507) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30506) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30506) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00030507()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 42 then
		return false;
	end
	if task:HasAcceptedTask(30507) or task:HasCompletedTask(30507) or task:HasSubmitedTask(30507) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30506) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30506) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00030507()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30507) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00030507(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30507) == npcId and Task_Accept_00030507 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30507
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç¬‘è€Œä¸è¯­";
	elseif task:GetTaskSubmitNpc(30507) == npcId then
		if Task_Submit_00030507() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30507
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç¬‘è€Œä¸è¯­";
		elseif task:HasAcceptedTask(30507) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30507
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç¬‘è€Œä¸è¯­";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00030507_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "å“ˆå“ˆï¼Œä½ ç»™æˆ‘å¸¦æ¥äº†å¦‚å½±é€æ¥çš„ç¾é…’å•Šï¼Œé‚£æˆ‘å¯è¦çè—å¥½ã€‚";
	action.m_ActionMsg = "å¦‚å½±è¿˜æƒ³è®©æˆ‘é—®ä½ ï¼Œä»–å‰é¢å—åˆ°åˆºå®¢çš„å·è¢­ï¼Œæ˜¯ä¸æ˜¯ä½ å»æ•‘å¥¹çš„ã€‚";
	return action;
end

function Task_00030507_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å•Šå“ˆå“ˆã€‚";
	action.m_ActionMsg = "å¦‚æœåœ¨ä¸‹æ²¡çŒœé”™ï¼Œåº”è¯¥æ˜¯ä½ ï¼Œæˆ‘ä»£ä»–å¤šè°¢ä½ çš„æ•‘å‘½ä¹‹æ©ã€‚";
	return action;
end

function Task_00030507_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å†ä¼šå•¦å‹‡å£«ï¼Œä½ èµ°ä½ çš„å±±æµ·è·¯ï¼Œæˆ‘å–æˆ‘çš„å¯‚å¯é…’ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030507_step_table = {
		[1] = Task_00030507_step_01,
		[2] = Task_00030507_step_02,
		[10] = Task_00030507_step_10,
		};

function Task_00030507_step(step)
	if Task_00030507_step_table[step] ~= nil then
		return Task_00030507_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00030507_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030507() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30507) then
		return false;
	end
	task:AddTaskStep(30507);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00030507_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30507) then
		return false;
	end


	player:AddExp(20000);
	player:getCoin(17500);
	player:getTael(30);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00030507_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30507);
end
