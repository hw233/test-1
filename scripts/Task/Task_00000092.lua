--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000092()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(92) or task:HasCompletedTask(92) or task:HasSubmitedTask(92) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(91) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(91) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(91) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000092()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(92) or task:HasCompletedTask(92) or task:HasSubmitedTask(92) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(91) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(91) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(91) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000092()
	if GetPlayer():GetTaskMgr():HasCompletedTask(92) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000092(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(92) == npcId and Task_Accept_00000092 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 92
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æš—ç®—å¤±æ‰‹";
	elseif task:GetTaskSubmitNpc(92) == npcId then
		if Task_Submit_00000092() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 92
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æš—ç®—å¤±æ‰‹";
		elseif task:HasAcceptedTask(92) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 92
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æš—ç®—å¤±æ‰‹";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000092_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ¡€æ¡€æ¡€ï¼Œå¾’å„¿ä½ è¿‡æ¥â€¦â€¦æ¥æ¥æ¥ï¼Œå†é è¿‘ä¸€ç‚¹ï¼Œæ€•ä»€ä¹ˆå‘€ï¼Œä¸ºå¸ˆæˆ‘è¿˜è¦ä¼ ä½ é­”åŠŸï¼Œåˆä¸ä¼šå®³ä½ ã€‚";
	action.m_ActionMsg = "å¸ˆå‚…æˆ‘æ€•â€¦â€¦ä½ ç¬‘å¾—è¿™ä¹ˆç‹°ç‹ï¼Œç›®éœ²å‡¶å…‰ï¼Œä¸€çœ‹å°±çŸ¥é“æ˜¯ä¸æ€€å¥½æ„ã€‚";
	return action;
end

function Task_00000092_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¥½ä½ ä¸ªå°è¾ˆï¼Œä½ ä»¥ä¸ºæˆ‘ç»¿è¢é‚£ä¹ˆç®€å•å°±è¢«ä½ ç»™éª—äº†ï¼";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000092_step_table = {
		[1] = Task_00000092_step_01,
		[10] = Task_00000092_step_10,
		};

function Task_00000092_step(step)
	if Task_00000092_step_table[step] ~= nil then
		return Task_00000092_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000092_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000092() then
		return false;
	end
	if not task:AcceptTask(92) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000092_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(92) then
		return false;
	end


	player:AddExp(28000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000092_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(92);
end
