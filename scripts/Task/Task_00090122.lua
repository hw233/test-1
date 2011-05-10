--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090122()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90122) or task:HasCompletedTask(90122) or task:HasSubmitedTask(90122) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90121) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90121) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090122()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90122) or task:HasCompletedTask(90122) or task:HasSubmitedTask(90122) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90121) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90121) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090122()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90122) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090122(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90122) == npcId and Task_Accept_00090122 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90122
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "é«˜çº§è­¦å¤‡";
	elseif task:GetTaskSubmitNpc(90122) == npcId then
		if Task_Submit_00090122() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90122
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "é«˜çº§è­¦å¤‡";
		elseif task:HasAcceptedTask(90122) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90122
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "é«˜çº§è­¦å¤‡";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090122_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ é—®ä¸‹å‹’å‹¤æ˜¯å¦çŸ¥é“ç‹¼äººå’Œé»‘é£æ€ªäº’ä¼ æƒ…æŠ¥ä¹‹äº‹ï¼Œè®©ä»–èµ¶ç´§åšé˜²å¤‡æªæ–½ï¼Œä»¥é˜²è¢«çªè¢­ã€‚";
	action.m_ActionMsg = "å¥½çš„ï¼Œå°†å†›è¯·æ”¾å¿ƒï¼Œæˆ‘å¿…å®šå®Œæˆé‡æ‰˜ã€‚";
	return action;
end

function Task_00090122_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å±…ç„¶æœ‰æ­¤äº‹ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090122_step_table = {
		[1] = Task_00090122_step_01,
		[10] = Task_00090122_step_10,
		};

function Task_00090122_step(step)
	if Task_00090122_step_table[step] ~= nil then
		return Task_00090122_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090122_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090122() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90122) then
		return false;
	end
	task:AddTaskStep(90122);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090122_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90122) then
		return false;
	end


	player:AddExp(700000);
	player:getCoin(316000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090122_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90122);
end
