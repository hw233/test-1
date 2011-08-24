--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000007()
	local player = GetPlayer();
	if player:GetLev() < 6 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(7) or task:HasCompletedTask(7) or task:HasSubmitedTask(7) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(6) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(6) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(6) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000007()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 6 then
		return false;
	end
	if task:HasAcceptedTask(7) or task:HasCompletedTask(7) or task:HasSubmitedTask(7) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(6) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(6) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(6) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000007()
	if GetPlayer():GetTaskMgr():HasCompletedTask(7) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000007(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(7) == npcId and Task_Accept_00000007 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 7
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å‰‘ä¾ å¿ƒæ³•";
	elseif task:GetTaskSubmitNpc(7) == npcId then
		if Task_Submit_00000007() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 7
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å‰‘ä¾ å¿ƒæ³•";
		elseif task:HasAcceptedTask(7) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 7
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å‰‘ä¾ å¿ƒæ³•";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000007_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ æœç„¶ä»™ç¼˜æ·±åšï¼Œè¿™æœ¬ä¹¦ç±ä¹ƒæ˜¯ä»™é“ä¸­äººä¿®ç‚¼çš„åŠŸæ³•ï¼Œå°‘ä¾ æœºç¼˜å·§åˆè·å¾—æ­¤ç‰©ï¼Œæƒ³å¿…ä¹Ÿæ˜¯å¤©æ„å¦‚æ­¤ï¼Œå¾…æˆ‘å‘Šè¯‰ä½ å¦‚ä½•ä½¿ç”¨è¿™åŠŸæ³•ï¼Œæ—¥åå¿…èƒ½æ–©å¦–é™¤é­”ï¼Œé€ ç¦è‹ç”Ÿã€‚";
	action.m_ActionMsg = "å¤šè°¢å‘¨å‰è¾ˆæŒ‡ç‚¹ã€‚";
	return action;
end

function Task_00000007_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¸Œæœ›å°‘ä¾ å°†æ¥å¯ä»¥é™¤é­”å«é“æ‹¯æ•‘ä¼—ç”Ÿã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000007_step_table = {
		[1] = Task_00000007_step_01,
		[10] = Task_00000007_step_10,
		};

function Task_00000007_step(step)
	if Task_00000007_step_table[step] ~= nil then
		return Task_00000007_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000007_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000007() then
		return false;
	end
	if not task:AcceptTask(7) then
		return false;
	end
	task:AddTaskStep(7);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000007_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(7) then
		return false;
	end


	player:AddExp(860);
	player:getTael(100);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000007_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(7);
end
