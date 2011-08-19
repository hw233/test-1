--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000085()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(85) or task:HasCompletedTask(85) or task:HasSubmitedTask(85) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(84) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(84) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(84) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000085()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(85) or task:HasCompletedTask(85) or task:HasSubmitedTask(85) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(84) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(84) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(84) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000085()
	if GetPlayer():GetTaskMgr():HasCompletedTask(85) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000085(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(85) == npcId and Task_Accept_00000085 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 85
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "éšå±…é«˜äºº";
	elseif task:GetTaskSubmitNpc(85) == npcId then
		if Task_Submit_00000085() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 85
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "éšå±…é«˜äºº";
		elseif task:HasAcceptedTask(85) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 85
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "éšå±…é«˜äºº";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000085_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é’èºé­”å®«é‡Œå¦–äººä¼—å¤šï¼Œä»…æˆ‘è¿™å¤©é­”å¤§æ³•æ˜¯è¿œè¿œä¸å¤Ÿçš„ã€‚ç¦»æ­¤ä¸è¿œå¤„æœ‰ä¸€ä¸ªç„å†°è°·ï¼Œè°·å†…éšå±…è¿™ä¸€ä¸ªå«å¥³æ®ƒç¥çš„å‰‘ä»™ï¼Œå¬è¯´æ˜”å¹´æ˜¯é‚ªé“ä¸­äººï¼Œå¦‚ä»Šå·²æ”¹é‚ªå½’æ­£ï¼Œè¯´ä¸å®šå¯ä»¥åŠ©æˆ‘ç­‰ç ´è¿™é’èºé­”å®«ã€‚";
	action.m_ActionMsg = "æ˜¯å—ï¼Œé‚£æˆ‘è¿™å°±å»è§è§è¿™ä½éšå±…é«˜äººã€‚";
	return action;
end

function Task_00000085_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è€èº«åœ¨æ­¤å‚æ¯ç¦…å·²æœ‰å¾ˆå¤šæ—¶æ—¥äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000085_step_table = {
		[1] = Task_00000085_step_01,
		[10] = Task_00000085_step_10,
		};

function Task_00000085_step(step)
	if Task_00000085_step_table[step] ~= nil then
		return Task_00000085_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000085_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000085() then
		return false;
	end
	if not task:AcceptTask(85) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(0,1);
	if itemNum ~= 0 then
		if itemNum > 0 then
			itemNum = 0;
			package:SetItem(0, itemNum, 1);
		end
		task:AddTaskStep2(85, 1, itemNum);
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000085_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(85) then
		return false;
	end


	player:AddExp(16000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000085_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(85);
end
