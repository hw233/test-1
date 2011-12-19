--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000188()
	local player = GetPlayer();
	if player:GetLev() < 90 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(188) or task:HasCompletedTask(188) or task:HasSubmitedTask(188) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(187) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(187) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(187) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000188()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 90 then
		return false;
	end
	if task:HasAcceptedTask(188) or task:HasCompletedTask(188) or task:HasSubmitedTask(188) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(187) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(187) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(187) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000188()
	if GetPlayer():GetTaskMgr():HasCompletedTask(188) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000188(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(188) == npcId and Task_Accept_00000188 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 188
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¤©æ± ä¸Šäºº";
	elseif task:GetTaskSubmitNpc(188) == npcId then
		if Task_Submit_00000188() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 188
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¤©æ± ä¸Šäºº";
		elseif task:HasAcceptedTask(188) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 188
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¤©æ± ä¸Šäºº";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000188_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘ä¹…å±…æ˜†ä»‘ï¼Œæœ‰ä¸€è¿‘é‚»ï¼Œå·å¤©æ± ä¸Šäººï¼Œæœ€è¿‘æœ‰äº›çƒ¦æ¼ï¼Œä½ å»çœ‹çœ‹å§ã€‚";
	action.m_ActionMsg = "å¥½å§ï¼Œæˆ‘å»å¸®åŠ©ä¸Šäººè§£å†³è¿™äº›çƒ¦æ¼ã€‚";
	return action;
end

function Task_00000188_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ å…‰ä¸´ï¼ŒçœŸæ˜¯è“¬èœç”Ÿè¾‰å•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000188_step_table = {
		[1] = Task_00000188_step_01,
		[10] = Task_00000188_step_10,
		};

function Task_00000188_step(step)
	if Task_00000188_step_table[step] ~= nil then
		return Task_00000188_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000188_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000188() then
		return false;
	end
	if not task:AcceptTask(188) then
		return false;
	end
	task:AddTaskStep(188);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000188_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(188) then
		return false;
	end


	player:AddExp(150000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000188_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(188);
end
