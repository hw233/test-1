--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080111()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80111) or task:HasCompletedTask(80111) or (task:HasSubmitedTask(80111) and (GetSharpDay(task:GetTaskEndTime(80111)) == GetSharpDay(os.time()))) then 
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00080111()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(80111) or task:HasCompletedTask(80111) or (task:HasSubmitedTask(80111) and (GetSharpDay(task:GetTaskEndTime(80111)) == GetSharpDay(os.time()))) then 
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00080111()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80111) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080111(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80111) == npcId and Task_Accept_00080111 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80111
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "è‹±é›„äº‰éœ¸";
	elseif task:GetTaskSubmitNpc(80111) == npcId then
		if Task_Submit_00080111() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80111
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "è‹±é›„äº‰éœ¸";
		elseif task:HasAcceptedTask(80111) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80111
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "è‹±é›„äº‰éœ¸";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080111_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å»ç«æŠ€åœºæ å¤ºåˆ«äººå§ï¼Œæƒ³æˆä¸ºè‹±é›„å°±åƒä¸‡ä¸è¦æ‰‹è½¯ã€‚";
	action.m_ActionMsg = "ä½ çœ‹æˆ‘å¯æ˜¯é‚£å¿ƒè½¯ä¹‹äººï¼Ÿæˆ‘ä»¬éœ€è¦çš„æ­£æ˜¯æ å¤ºçš„é‡å¿ƒã€‚";
	return action;
end

function Task_00080111_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‹‡å£«ï¼Œæƒ…å†µå¦‚ä½•ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080111_step_table = {
		[1] = Task_00080111_step_01,
		[10] = Task_00080111_step_10,
		};

function Task_00080111_step(step)
	if Task_00080111_step_table[step] ~= nil then
		return Task_00080111_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080111_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080111() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80111) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00080111_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80111) then
		return false;
	end


	player:AddExp(30000);
	player:getTael(50);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080111_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80111);
end
