--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090102()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90102) or task:HasCompletedTask(90102) or task:HasSubmitedTask(90102) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90101) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90101) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090102()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90102) or task:HasCompletedTask(90102) or task:HasSubmitedTask(90102) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90101) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90101) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090102()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90102) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090102(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90102) == npcId and Task_Accept_00090102 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90102
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç¾å¥½çš„æœŸæœ›";
	elseif task:GetTaskSubmitNpc(90102) == npcId then
		if Task_Submit_00090102() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90102
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç¾å¥½çš„æœŸæœ›";
		elseif task:HasAcceptedTask(90102) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90102
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç¾å¥½çš„æœŸæœ›";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090102_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "æˆ˜äº‰ä½•æ—¶ä¼‘ï¼";
	action.m_ActionMsg = "å¤§å®¶éƒ½å¸Œæœ›ä¸–ç•Œæ˜¯å’Œå¹³çš„ï¼Œå¯é‚£äº›å¦–é­”é¬¼æ€ªæ€»è¦ç ´åä¸–é—´å’Œå¹³ã€‚";
	return action;
end

function Task_00090102_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ”¾ä¸‹å± åˆ€ï¼Œç«‹åœ°æˆä½›ï¼Œæ–¹å¯å’Œå¹³ã€‚";
	action.m_ActionMsg = "æˆ‘ä¼šå°†é•¿è€çš„è¯å¸¦åˆ°ã€‚";
	return action;
end

function Task_00090102_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è°ä¹Ÿä¸æƒ³å¸¸å¹´å¾æˆ˜ï¼Œè°éƒ½æƒ³å¤©ä¼¦ä¹‹ä¹ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090102_step_table = {
		[1] = Task_00090102_step_01,
		[2] = Task_00090102_step_02,
		[10] = Task_00090102_step_10,
		};

function Task_00090102_step(step)
	if Task_00090102_step_table[step] ~= nil then
		return Task_00090102_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090102_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090102() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90102) then
		return false;
	end
	task:AddTaskStep(90102);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090102_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90102) then
		return false;
	end


	player:AddExp(500000);
	player:getCoin(300000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090102_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90102);
end
