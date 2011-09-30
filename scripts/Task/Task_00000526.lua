--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000526()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(526) or task:HasCompletedTask(526) or task:HasSubmitedTask(526) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000526()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(526) or task:HasCompletedTask(526) or task:HasSubmitedTask(526) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000526()
	if GetPlayer():GetTaskMgr():HasCompletedTask(526) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000526(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(526) == npcId and Task_Accept_00000526 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 526
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ•™è¯²";
	elseif task:GetTaskSubmitNpc(526) == npcId then
		if Task_Submit_00000526() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 526
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ•™è¯²";
		elseif task:HasAcceptedTask(526) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 526
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ•™è¯²";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000526_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."æˆ‘ä»¬å³¨çœ‰å¼€åºœåœ¨å³ï¼Œä¹Ÿæ˜¯å¤©ä¸‹æ­£é“çš„é¢†è¢–ï¼Œä½œä¸ºå³¨çœ‰å¼Ÿå­ä¸€å®šè¦æ—¶åˆ»è‡ªçœå…¶èº«ã€‚ç„çœŸå­å¸ˆä¼¯æ—©åœ¨æˆ‘å…¥é—¨å‰å°±è·Ÿéšä½ å¸ˆç¥–é•¿çœ‰çœŸäººï¼Œå—è®­é¢‡ä¸¥ï¼Œä»Šæ—¥ä½ å°±å»å¸ˆä¼¯å¤„è†å¬æ•™è¯²å§ã€‚";
	action.m_ActionMsg = "è°¨éµæŒæ•™ä¹‹ä»¤ã€‚";
	return action;
end

function Task_00000526_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘çœ‹ä½ å¿ƒå­˜æ­£æ°”ï¼Œå°†æ¥å¿…ç„¶ä¼šå°†æˆ‘æ´¾å‘æ‰¬å…‰å¤§ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000526_step_table = {
		[1] = Task_00000526_step_01,
		[10] = Task_00000526_step_10,
		};

function Task_00000526_step(step)
	if Task_00000526_step_table[step] ~= nil then
		return Task_00000526_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000526_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000526() then
		return false;
	end
	if not task:AcceptTask(526) then
		return false;
	end
	task:AddTaskStep(526);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000526_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(526) then
		return false;
	end


	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000526_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(526);
end
