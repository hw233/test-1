--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000112()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(112) or task:HasCompletedTask(112) or task:HasSubmitedTask(112) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(111) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(111) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(111) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000112()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(112) or task:HasCompletedTask(112) or task:HasSubmitedTask(112) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(111) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(111) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(111) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000112()
	if GetPlayer():GetTaskMgr():HasCompletedTask(112) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000112(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(112) == npcId and Task_Accept_00000112 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 112
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å†æˆ˜ç»¿è¢";
	elseif task:GetTaskSubmitNpc(112) == npcId then
		if Task_Submit_00000112() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 112
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å†æˆ˜ç»¿è¢";
		elseif task:HasAcceptedTask(112) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 112
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å†æˆ˜ç»¿è¢";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000112_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘ä»¬å·²ç»å¸ƒä¸‹ä¸¤ä»ªå¾®å°˜å¤§é˜µå°†ç»¿è¢çš„å·¢ç©´é˜´é£æ´å›¢å›¢å›´ä½ï¼Œåˆ«è¯´ä»–æœ‰ç„ç‰ç ï¼Œè¿™å›å°±ç®—ä»–æ’ç¿…ä¹Ÿéš¾é€ƒä¸€æ­»ï¼Œ"..GetPlayerName(GetPlayer()).."ä½ å°±å’Œæˆ‘ä»¬ä¸€é“å»ç­æ­¤å¦–å­½ï¼Œä¹Ÿè®©ä½ è§è¯†ä¸€ä¸‹ä¸¤ä»ªå¾®å°˜é˜µçš„ç„å¥¥ä¹‹å¤„ã€‚";
	action.m_ActionMsg = "æ˜¯å—ï¼Œé‚£å¤ªå¥½äº†ã€‚";
	return action;
end

function Task_00000112_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¦‚ä»Šç»¿è¢å¾—è¯›ï¼Œç®—æ˜¯åŠŸè¡Œåœ†æ»¡äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000112_step_table = {
		[1] = Task_00000112_step_01,
		[10] = Task_00000112_step_10,
		};

function Task_00000112_step(step)
	if Task_00000112_step_table[step] ~= nil then
		return Task_00000112_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000112_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000112() then
		return false;
	end
	if not task:AcceptTask(112) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000112_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(112) then
		return false;
	end


	player:AddExp(98888);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000112_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(112);
end
