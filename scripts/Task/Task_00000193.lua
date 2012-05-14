--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000193()
	local player = GetPlayer();
	if player:GetLev() < 95 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(193) or task:HasCompletedTask(193) or task:HasSubmitedTask(193) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(192) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(192) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(192) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000193()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 95 then
		return false;
	end
	if task:HasAcceptedTask(193) or task:HasCompletedTask(193) or task:HasSubmitedTask(193) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(192) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(192) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(192) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000193()
	if GetPlayer():GetTaskMgr():HasCompletedTask(193) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000193(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(193) == npcId and Task_Accept_00000193 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 193
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¿å¤§å¸ˆ";
	elseif task:GetTaskSubmitNpc(193) == npcId then
		if Task_Submit_00000193() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 193
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¿å¤§å¸ˆ";
		elseif task:HasAcceptedTask(193) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 193
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¿å¤§å¸ˆ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000193_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ æŠ€è‰ºéå‡¡å•Šï¼Œæˆ‘çœ‹ä½ æ¯”ä¸‰è‹±äºŒäº‘é‡Œé¢æè‹±ç¼é‚£å¨ƒå„¿éƒ½ä¸å·®ï¼Œä½ çœ‹å¯¹é¢æ‹¿å‡ºå±±å³°å«åšå°å¯’å±±ï¼Œå…¶ä¸­éšå±…ç€ä¸€ä½ç¥å°¼å¿å¤§å¸ˆï¼Œä½ å»æ‹œè§ä¸€ä¸‹å§ã€‚";
	action.m_ActionMsg = "æˆ‘è¿™å°±å»æ‹œè§å¿å¤§å¸ˆã€‚";
	return action;
end

function Task_00000193_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ çœŸæ˜¯ä¿®ä¸ºæ·±åšï¼Œå¤©èµ‹å¼‚ç¦€å•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000193_step_table = {
		[1] = Task_00000193_step_01,
		[10] = Task_00000193_step_10,
		};

function Task_00000193_step(step)
	if Task_00000193_step_table[step] ~= nil then
		return Task_00000193_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000193_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000193() then
		return false;
	end
	if not task:AcceptTask(193) then
		return false;
	end
	task:AddTaskStep(193);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000193_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(193) then
		return false;
	end


	player:AddExp(180000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000193_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(193);
end
