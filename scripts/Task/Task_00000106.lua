--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000106()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(106) or task:HasCompletedTask(106) or task:HasSubmitedTask(106) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000106()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(106) or task:HasCompletedTask(106) or task:HasSubmitedTask(106) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000106()
	if GetPlayer():GetTaskMgr():HasCompletedTask(106) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000106(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(106) == npcId and Task_Accept_00000106 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 106
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "106";
	elseif task:GetTaskSubmitNpc(106) == npcId then
		if Task_Submit_00000106() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 106
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "106";
		elseif task:HasAcceptedTask(106) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 106
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "106";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000106_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ å¥½";
	action.m_ActionMsg = "æˆ‘ä¸å¥½";
	return action;
end

function Task_00000106_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ å¥½";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000106_step_table = {
		[1] = Task_00000106_step_01,
		[10] = Task_00000106_step_10,
		};

function Task_00000106_step(step)
	if Task_00000106_step_table[step] ~= nil then
		return Task_00000106_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000106_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000106() then
		return false;
	end
	if not task:AcceptTask(106) then
		return false;
	end
	task:AddTaskStep(106);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000106_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(106) then
		if DayTaskAward(0) then
			task:DayTaskSubmit(106);
			return true;
		end
	end

	return false;
end

--·ÅÆúÈÎÎñ
function Task_00000106_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(106);
end
