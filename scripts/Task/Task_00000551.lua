--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000551()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(551) or task:HasCompletedTask(551) or task:HasSubmitedTask(551) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000551()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(551) or task:HasCompletedTask(551) or task:HasSubmitedTask(551) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000551()
	if GetPlayer():GetTaskMgr():HasCompletedTask(551) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000551(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(551) == npcId and Task_Accept_00000551 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 551
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ•™è¯²";
	elseif task:GetTaskSubmitNpc(551) == npcId then
		if Task_Submit_00000551() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 551
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ•™è¯²";
		elseif task:HasAcceptedTask(551) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 551
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
function Task_00000551_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."æˆ‘ä»¬æ˜†ä»‘æ´¾æºè¿œæµé•¿ï¼Œä¹Ÿæ˜¯å¤©ä¸‹æ­£é“æ°”è¿æ‰€åœ¨ï¼Œä½œä¸ºæ˜†ä»‘å¼Ÿå­ä¸€å®šè¦æ—¶åˆ»è‡ªçœå…¶èº«ã€‚ä½ å¸ˆå”éŸ¦å°‘å°‘å…¥æ˜†ä»‘å·²ä¹…ï¼Œå—è®­é¢‡ä¸¥ï¼Œä»Šæ—¥ä½ å°±å»å¸ˆå”å¤„è†å¬æ•™è¯²å§ã€‚";
	action.m_ActionMsg = "è°¨éµæŒæ•™ä¹‹ä»¤ã€‚";
	return action;
end

function Task_00000551_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘çœ‹ä½ å¿ƒå­˜æ­£æ°”ï¼Œå°†æ¥å¿…ç„¶ä¼šå°†æˆ‘æ´¾å‘æ‰¬å…‰å¤§ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000551_step_table = {
		[1] = Task_00000551_step_01,
		[10] = Task_00000551_step_10,
		};

function Task_00000551_step(step)
	if Task_00000551_step_table[step] ~= nil then
		return Task_00000551_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000551_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000551() then
		return false;
	end
	if not task:AcceptTask(551) then
		return false;
	end
	task:AddTaskStep(551);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000551_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(551) then
		return false;
	end


	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000551_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(551);
end
