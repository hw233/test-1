--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00040101()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 8 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40101) or task:HasCompletedTask(40101) or task:HasSubmitedTask(40101) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00040101()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 8 then
		return false;
	end
	if task:HasAcceptedTask(40101) or task:HasCompletedTask(40101) or task:HasSubmitedTask(40101) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00040101()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40101) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00040101(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40101) == npcId and Task_Accept_00040101 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40101
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¿§éƒçš„ç‹å¦ƒ";
	elseif task:GetTaskSubmitNpc(40101) == npcId then
		if Task_Submit_00040101() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40101
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¿§éƒçš„ç‹å¦ƒ";
		elseif task:HasAcceptedTask(40101) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40101
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¿§éƒçš„ç‹å¦ƒ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00040101_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "å‹‡å£«ï¼Œå¦‚æœä½ ç°åœ¨æœ‰æ—¶é—´èƒ½å¸®æˆ‘ä¸ªå¿™å—ï¼Ÿ";
	action.m_ActionMsg = "æ‚¨è¯´å§ã€‚";
	return action;
end

function Task_00040101_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ èƒ½å¸®æˆ‘å»é—®ä¸€ä¸‹æˆ‘ä»¬çš„å¸é‡Šå¤©ï¼Œä»Šæ™šæ˜¯æœˆåœ†ä¹‹å¤œï¼Œä»–æ„¿ä¸æ„¿æ„æ¥é™ªæˆ‘å»èµæœˆå‘¢ï¼Ÿ";
	action.m_ActionMsg = "ç‹å¦ƒï¼Œä¸æ˜¯æˆ‘ä¸æ„¿æ„ï¼Œè¿™ç‚¹å°äº‹æ‚¨ä¸ºä»€ä¹ˆä¸èƒ½è‡ªå·±å»é—®å‘¢ï¼Ÿ";
	return action;
end

function Task_00040101_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‹‡å£«ï¼Œä½ æ€ä¹ˆæ˜¯ä»ç‹å¦ƒé‚£é‡Œè¿‡æ¥çš„ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040101_step_table = {
		[1] = Task_00040101_step_01,
		[2] = Task_00040101_step_02,
		[10] = Task_00040101_step_10,
		};

function Task_00040101_step(step)
	if Task_00040101_step_table[step] ~= nil then
		return Task_00040101_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00040101_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040101() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40101) then
		return false;
	end
	task:AddTaskStep(40101);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00040101_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40101) then
		return false;
	end


	player:AddExp(800);
	player:getCoin(500);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00040101_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40101);
end
