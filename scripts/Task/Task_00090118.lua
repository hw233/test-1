--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090118()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90118) or task:HasCompletedTask(90118) or task:HasSubmitedTask(90118) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90117) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90117) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090118()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90118) or task:HasCompletedTask(90118) or task:HasSubmitedTask(90118) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90117) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90117) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090118()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90118) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090118(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90118) == npcId and Task_Accept_00090118 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90118
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ—§äº‹";
	elseif task:GetTaskSubmitNpc(90118) == npcId then
		if Task_Submit_00090118() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90118
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ—§äº‹";
		elseif task:HasAcceptedTask(90118) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90118
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ—§äº‹";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090118_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "çœ‹æ¥ä»–è¿˜æ˜¯å¿˜ä¸äº†æ—§äº‹ã€‚";
	action.m_ActionMsg = "ä»€ä¹ˆæ—§äº‹ï¼Ÿ";
	return action;
end

function Task_00090118_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ï¼ˆå¹æ°”ï¼‰ï¼Œä¸æä¹Ÿç½¢ã€‚æ—¢ç„¶ä»–ä¸æ„¿æ„æ¥ï¼Œé‚£è¿™å„¿è¿˜å¾—è¯·å‹‡å£«å¤šç•™é˜µå­ï¼Œå¸®æˆ‘ä»¬æ‰«æ¸…æ•Œå¯‡ã€‚";
	action.m_ActionMsg = "è¿™äº›éƒ½æ˜¯å°äº‹ï¼Œåªæ˜¯å¯¹å°†å†›å’Œå•å…ˆç”Ÿä¹‹é—´çš„æ—§äº‹å¾ˆå¥½å¥‡ï¼Œéš¾åˆ°æ˜¯ä¸ºäº†è²‚è‰ï¼Ÿæˆ‘æƒ³å°†å†›ä¸æ˜¯å¥½è‰²ä¹‹äººã€‚";
	return action;
end

function Task_00090118_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¯·å‹‡å£«ä¸è¦å¤šæƒ³ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090118_step_table = {
		[1] = Task_00090118_step_01,
		[2] = Task_00090118_step_02,
		[10] = Task_00090118_step_10,
		};

function Task_00090118_step(step)
	if Task_00090118_step_table[step] ~= nil then
		return Task_00090118_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090118_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090118() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90118) then
		return false;
	end
	task:AddTaskStep(90118);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090118_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90118) then
		return false;
	end


	player:AddExp(650000);
	player:getCoin(312000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090118_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90118);
end
