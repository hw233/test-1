--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000036()
	local player = GetPlayer();
	if player:GetLev() < 24 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(36) or task:HasCompletedTask(36) or task:HasSubmitedTask(36) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(35) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(35) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(35) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000036()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 24 then
		return false;
	end
	if task:HasAcceptedTask(36) or task:HasCompletedTask(36) or task:HasSubmitedTask(36) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(35) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(35) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(35) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000036()
	if GetPlayer():GetTaskMgr():HasCompletedTask(36) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000036(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(36) == npcId and Task_Accept_00000036 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 36
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æˆéƒ½åºœè¡™";
	elseif task:GetTaskSubmitNpc(36) == npcId then
		if Task_Submit_00000036() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 36
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æˆéƒ½åºœè¡™";
		elseif task:HasAcceptedTask(36) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 36
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æˆéƒ½åºœè¡™";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000036_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™ä¸ªæ¶å¾’ç™¾æ­»ä¸å¾—èµå…¶ç½ªï¼Œä¸è¿‡å°‘ä¾ è¿˜æ˜¯å°†ä»–é€åˆ°è¡™é—¨é‡Œå§ï¼Œè®©å®˜åºœæ¥å®šä»–çš„ç½ªã€‚";
	action.m_ActionMsg = "å¥½å§ã€‚";
	return action;
end

function Task_00000036_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ çœŸæ˜¯èº«æ‰‹ä¸å‡¡å•Šï¼Œå¼ äº®è¿™ä¸ªæ¶å¾’å®˜åºœæ—©å·²é€šç¼‰åœ¨æ¡ˆï¼Œåªæ˜¯è¿™å®è½»åŠŸé«˜å¼ºï¼Œä¸€ç›´ä¸è§ä»–è¸ªè¿¹ï¼Œå´ä¸æ–™è¢«å°‘ä¾ ä½ ç»™æŠ“ä½äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000036_step_table = {
		[1] = Task_00000036_step_01,
		[10] = Task_00000036_step_10,
		};

function Task_00000036_step(step)
	if Task_00000036_step_table[step] ~= nil then
		return Task_00000036_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000036_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000036() then
		return false;
	end
	if not task:AcceptTask(36) then
		return false;
	end
	task:AddTaskStep(36);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000036_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(36) then
		return false;
	end


	player:AddExp(3333);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000036_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(36);
end
