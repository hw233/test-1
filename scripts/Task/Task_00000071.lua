--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000071()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(71) or task:HasCompletedTask(71) or task:HasSubmitedTask(71) then
		return false;
	end
	if not task:HasSubmitedTask(50) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000071()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(71) or task:HasCompletedTask(71) or task:HasSubmitedTask(71) then
		return false;
	end
	if not task:HasSubmitedTask(50) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000071()
	if GetPlayer():GetTaskMgr():HasCompletedTask(71) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000071(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(71) == npcId and Task_Accept_00000071 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 71
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æŒæ•™çœŸäºº";
	elseif task:GetTaskSubmitNpc(71) == npcId then
		if Task_Submit_00000071() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 71
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æŒæ•™çœŸäºº";
		elseif task:HasAcceptedTask(71) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 71
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æŒæ•™çœŸäºº";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000071_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ…ˆäº‘å¯ºä¸€æˆ˜å·²ç»ç»ˆäº†ï¼Œå¤§ä¼™éƒ½é½èšèœ€å±±ï¼Œ<name>ä½ ä¹Ÿä¸€èµ·å»å§ã€‚æ˜†ä»‘æŒæ•™ä¸€å…ƒçœŸäººåœ¨æ˜†ä»‘æ´¾çš„ä¸Šæ¸…å®«ç­‰ç€è§è§ä½ è¿™ä¸ªæ­£é“çš„åèµ·ä¹‹ç§€å‘¢ã€‚";
	action.m_ActionMsg = "å—¯ï¼Œæˆ‘è¿™å°±èµ¶å»ã€‚";
	return action;
end

function Task_00000071_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "<name>æ­£é“ä¸­å…´çš„é‡æ‹…å°±åœ¨ä½ ä»¬è¿™å°ä¸€è¾ˆçš„è‚©ä¸Šäº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000071_step_table = {
		[1] = Task_00000071_step_01,
		[10] = Task_00000071_step_10,
		};

function Task_00000071_step(step)
	if Task_00000071_step_table[step] ~= nil then
		return Task_00000071_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000071_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000071() then
		return false;
	end
	if not task:AcceptTask(71) then
		return false;
	end
	task:AddTaskStep(71);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000071_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(71) then
		return false;
	end


	player:AddExp(11111);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000071_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(71);
end
