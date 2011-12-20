--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000187()
	local player = GetPlayer();
	if player:GetLev() < 90 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(187) or task:HasCompletedTask(187) or task:HasSubmitedTask(187) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(186) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(186) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(186) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000187()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 90 then
		return false;
	end
	if task:HasAcceptedTask(187) or task:HasCompletedTask(187) or task:HasSubmitedTask(187) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(186) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(186) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(186) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000187()
	if GetPlayer():GetTaskMgr():HasCompletedTask(187) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000187(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(187) == npcId and Task_Accept_00000187 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 187
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¼‚å…½ç„æ­¦";
	elseif task:GetTaskSubmitNpc(187) == npcId then
		if Task_Submit_00000187() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 187
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¼‚å…½ç„æ­¦";
		elseif task:HasAcceptedTask(187) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 187
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¼‚å…½ç„æ­¦";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000187_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ˜†ä»‘ä¹ƒæ˜¯ä¸Šå¤ç¥äººä¹‹åœ°ï¼Œæœ‰ä¸å°‘å¼‚å…½æ®‹å­˜ï¼Œä»–ä»¬ä¿®ç‚¼å¤šå¹´ï¼Œé“è¡Œé«˜æ·±ï¼Œå°‘ä¾ å¯ä»¥å»æ‹¿ä»–ä»¬ç»ƒç»ƒæ‰‹ã€‚æ˜†ä»‘æ³‰å¤„æœ‰ä¸€åªç„æ­¦ï¼Œå°‘ä¾ å»è¯•è¯•å§ã€‚";
	action.m_ActionMsg = "æˆ‘å»ä¼šä¼šè¿™äº›ä¸Šå¤å¼‚å…½ã€‚";
	return action;
end

function Task_00000187_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ çœŸæ˜¯ä¿®ä¸ºæ·±åšï¼Œå¤©èµ‹å¼‚ç¦€å•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000187_step_table = {
		[1] = Task_00000187_step_01,
		[10] = Task_00000187_step_10,
		};

function Task_00000187_step(step)
	if Task_00000187_step_table[step] ~= nil then
		return Task_00000187_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000187_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000187() then
		return false;
	end
	if not task:AcceptTask(187) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000187_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(187) then
		return false;
	end


	player:AddExp(150000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000187_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(187);
end
