--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00070301()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(70301) or task:HasCompletedTask(70301) or task:HasSubmitedTask(70301) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00070301()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(70301) or task:HasCompletedTask(70301) or task:HasSubmitedTask(70301) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	if task:GetDayTaskCurrTaskId(1003) ~= 70301 then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00070301()
	if GetPlayer():GetTaskMgr():HasCompletedTask(70301) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00070301(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(70301) == npcId and Task_Accept_00070301 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 70301
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¤§ä¾ ä¸­çš„æˆ˜æ–—æœº";
	elseif task:GetTaskSubmitNpc(70301) == npcId then
		if Task_Submit_00070301() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70301
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¤§ä¾ ä¸­çš„æˆ˜æ–—æœº";
		elseif task:HasAcceptedTask(70301) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70301
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¤§ä¾ ä¸­çš„æˆ˜æ–—æœº";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00070301_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä¾ ä¸­ä¹‹å¤§ï¼Œå°±æ˜¯è¦ä¸ç•ä»»ä½•å±é™©å’Œå›°éš¾ã€‚ç¿å¢å²­çš„ç»ä¸–æ—æ­£æ˜¯ä¸€ä¸ªèƒ½å¤Ÿè¯æ˜ä½ çš„åœ°æ–¹ï¼Œæ‰“è´¥é‚£é‡Œçš„æ€¨çµå°„æ‰‹ï¼Œä½ å°±æ˜¯å¤§ä¾ ä¸­çš„æˆ˜æ–—æœºã€‚";
	action.m_ActionMsg = "é‚£äº›å°å®¶ä¼™ä½ ä¹Ÿè®©æˆ‘å»æ‰“?é‚£å¥½å§.";
	return action;
end

function Task_00070301_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è†œæ‹œå•Šï¼Œå¤§ä¾ ä¸­çš„æˆ˜æ–—æœºã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00070301_step_table = {
		[1] = Task_00070301_step_01,
		[10] = Task_00070301_step_10,
		};

function Task_00070301_step(step)
	if Task_00070301_step_table[step] ~= nil then
		return Task_00070301_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00070301_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00070301() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(70301) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00070301_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(70301) then
		if DayTaskAward(1003) then
			task:DayTaskSubmit(70301);
			return true;
		end
	end

	return false;
end

--·ÅÆúÈÎÎñ
function Task_00070301_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(70301);
end
