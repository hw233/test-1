--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000009()
	local player = GetPlayer();
	if player:GetLev() < 9 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(9) or task:HasCompletedTask(9) or task:HasSubmitedTask(9) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(8) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(8) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(8) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000009()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 9 then
		return false;
	end
	if task:HasAcceptedTask(9) or task:HasCompletedTask(9) or task:HasSubmitedTask(9) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(8) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(8) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(8) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000009()
	if GetPlayer():GetTaskMgr():HasCompletedTask(9) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000009(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(9) == npcId and Task_Accept_00000009 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 9
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "äº‹å‘çªç„¶";
	elseif task:GetTaskSubmitNpc(9) == npcId then
		if Task_Submit_00000009() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 9
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "äº‹å‘çªç„¶";
		elseif task:HasAcceptedTask(9) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 9
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "äº‹å‘çªç„¶";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000009_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘å’Œçˆ¶äº²åœ¨èˆèº«å´–æ‰ç¨æ¯äº†ç‰‡åˆ»ï¼Œå°±çœ‹åˆ°èµµç‡•å„¿è·‘æ¥ï¼Œååˆ†ç„¦æ€¥ï¼Œä¼¼ä¹æ‰¾ä»–å¸ˆå‚…æœ‰æ€¥äº‹ï¼Œå°‘ä¾ ä¸å¦¨å»é—®é—®ä»–ï¼Œåˆ°åº•æ˜¯ä½•äº‹å¦‚æ­¤æƒŠæ…Œï¼Ÿ";
	action.m_ActionMsg = "å—¯ï¼Œæˆ‘å»é—®é—®ã€‚";
	return action;
end

function Task_00000009_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¯çœ‹è§æˆ‘å¸ˆå‚…åœ¨å“ªï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000009_step_table = {
		[1] = Task_00000009_step_01,
		[10] = Task_00000009_step_10,
		};

function Task_00000009_step(step)
	if Task_00000009_step_table[step] ~= nil then
		return Task_00000009_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000009_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000009() then
		return false;
	end
	if not task:AcceptTask(9) then
		return false;
	end
	task:AddTaskStep(9);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000009_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(9) then
		return false;
	end


	player:AddExp(1000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000009_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(9);
end
