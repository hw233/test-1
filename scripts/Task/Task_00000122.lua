--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000122()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(122) or task:HasCompletedTask(122) or task:HasSubmitedTask(122) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(5) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(5) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(5) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000122()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(122) or task:HasCompletedTask(122) or task:HasSubmitedTask(122) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(5) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(5) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(5) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000122()
	if GetPlayer():GetTaskMgr():HasCompletedTask(122) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000122(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(122) == npcId and Task_Accept_00000122 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 122
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç¥ç§˜å¤ç±";
	elseif task:GetTaskSubmitNpc(122) == npcId then
		if Task_Submit_00000122() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 122
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç¥ç§˜å¤ç±";
		elseif task:HasAcceptedTask(122) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 122
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç¥ç§˜å¤ç±";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000122_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™æœ¬å¤ç±çœ‹èµ·æ¥ä¼¼ä¹ä¸ä¸€èˆ¬ï¼Œå¾…æˆ‘çœ‹çœ‹â€¦â€¦å“å‘€ï¼è¿™å¥½åƒæ˜¯ä¼ è¯´ä¸­å‰‘ä»™ä¸€æµå­¦ä¹ çš„ç§˜ç±å¿ƒæ³•å‘€ï¼Œ"..GetPlayerName(GetPlayer()).."ä½ çœŸæ˜¯ç¦ç¼˜æ·±åšï¼Œçœ‹æ¥å°‘ä¾ ä½ æ˜¯ä¸è¿™æœ¬ç¥ç§˜çš„å¤ç±æœ‰ç¼˜ï¼Œä¸å¦‚å°±å°†å®ƒå­¦ä¼šäº†å§ã€‚";
	action.m_ActionMsg = "å“ªé‡Œï¼Œå“ªé‡Œâ€¦â€¦";
	return action;
end

function Task_00000122_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ çœŸæ˜¯ç¦ç¼˜æ·±åšå‘€ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000122_step_table = {
		[1] = Task_00000122_step_01,
		[10] = Task_00000122_step_10,
		};

function Task_00000122_step(step)
	if Task_00000122_step_table[step] ~= nil then
		return Task_00000122_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000122_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000122() then
		return false;
	end
	if not task:AcceptTask(122) then
		return false;
	end
	task:AddTaskStep(122);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000122_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(122) then
		return false;
	end


	player:AddExp(600);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000122_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(122);
end
