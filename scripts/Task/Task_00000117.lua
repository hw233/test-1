--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000117()
	local player = GetPlayer();
	if player:GetLev() < 60 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(117) or task:HasCompletedTask(117) or task:HasSubmitedTask(117) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(116) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(116) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(116) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000117()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 60 then
		return false;
	end
	if task:HasAcceptedTask(117) or task:HasCompletedTask(117) or task:HasSubmitedTask(117) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(116) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(116) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(116) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000117()
	if GetPlayer():GetTaskMgr():HasCompletedTask(117) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000117(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(117) == npcId and Task_Accept_00000117 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 117
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "é’å›Šä»™å­";
	elseif task:GetTaskSubmitNpc(117) == npcId then
		if Task_Submit_00000117() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 117
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "é’å›Šä»™å­";
		elseif task:HasAcceptedTask(117) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 117
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "é’å›Šä»™å­";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000117_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ–¹æ‰æˆ‘è§åˆ°å³¨çœ‰æ¥äººï¼Œå¥½åƒæ˜¯ä¸ªé“å§‘ï¼Œå»äº†é‚£è¾¹çš„ä¸€å¤„å¥¥åŒºä»™åºœï¼Œå°‘ä¾ ä½ å»è§è§å¥¹å§ã€‚";
	action.m_ActionMsg = "å¥½çš„ï¼Œæˆ‘è¿™å°±å»è§å‰è¾ˆã€‚";
	return action;
end

function Task_00000117_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä¹…é—»å°‘ä¾ ä¹‹åï¼Œä»Šæ—¥å¾—è§æœç„¶åä¸è™šä¼ ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000117_step_table = {
		[1] = Task_00000117_step_01,
		[10] = Task_00000117_step_10,
		};

function Task_00000117_step(step)
	if Task_00000117_step_table[step] ~= nil then
		return Task_00000117_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000117_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000117() then
		return false;
	end
	if not task:AcceptTask(117) then
		return false;
	end
	task:AddTaskStep(117);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000117_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(117) then
		return false;
	end


	player:AddExp(36000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000117_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(117);
end
