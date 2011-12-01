--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000078()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(78) or task:HasCompletedTask(78) or task:HasSubmitedTask(78) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(52) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(52) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(52) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000078()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(78) or task:HasCompletedTask(78) or task:HasSubmitedTask(78) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(52) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(52) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(52) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000078()
	if GetPlayer():GetTaskMgr():HasCompletedTask(78) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000078(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(78) == npcId and Task_Accept_00000078 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 78
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å³¨çœ‰è®¿å®¢";
	elseif task:GetTaskSubmitNpc(78) == npcId then
		if Task_Submit_00000078() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 78
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å³¨çœ‰è®¿å®¢";
		elseif task:HasAcceptedTask(78) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 78
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å³¨çœ‰è®¿å®¢";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000078_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æœ‰åŠ³å°‘ä¾ äº†ä¸ºæˆ‘ä»¬æˆ´å®¶åœºå¦‚æ­¤å¥”æ³¢äº†ï¼Œæ–¹æ‰æœ‰ä¸€ä½å³¨çœ‰çš„å‰è¾ˆé«˜äººæ¥è®¿ï¼Œè¯´æœ‰äº‹æ‰¾ä½ ï¼Œç°ä¸‹ä»–æ­£åœ¨æˆ´å®¶çš„å¤§é™¢é‡Œç­‰å€™ï¼Œå°‘ä¾ è¿˜æ˜¯å¿«å¿«å»å§ã€‚";
	action.m_ActionMsg = "æ˜¯å—ï¼Œæˆ‘è¿™å°±å»ã€‚";
	return action;
end

function Task_00000078_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æƒ³ä¸åˆ°æˆ‘æ˜”æ—¥æ•™æˆçš„æ¶å¾’ç°åœ¨ç«Ÿç„¶åœ¨æ­¤ä¸ºæ¶ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000078_step_table = {
		[1] = Task_00000078_step_01,
		[10] = Task_00000078_step_10,
		};

function Task_00000078_step(step)
	if Task_00000078_step_table[step] ~= nil then
		return Task_00000078_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000078_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000078() then
		return false;
	end
	if not task:AcceptTask(78) then
		return false;
	end
	task:AddTaskStep(78);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000078_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(78) then
		return false;
	end


	player:AddExp(10000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000078_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(78);
end
