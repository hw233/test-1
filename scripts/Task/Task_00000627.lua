--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000627()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(627) or task:HasCompletedTask(627) or task:HasSubmitedTask(627) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000627()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(627) or task:HasCompletedTask(627) or task:HasSubmitedTask(627) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000627()
	if GetPlayer():GetTaskMgr():HasCompletedTask(627) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000627(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(627) == npcId and Task_Accept_00000627 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 627
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ°´è´¼å–½å•°";
	elseif task:GetTaskSubmitNpc(627) == npcId then
		if Task_Submit_00000627() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 627
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ°´è´¼å–½å•°";
		elseif task:HasAcceptedTask(627) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 627
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ°´è´¼å–½å•°";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000627_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆéƒ½å‰æ®µæ—¶é—´æ²»å®‰çœŸå¾—å¾ˆæˆé—®é¢˜ï¼Œæœ‰ä¸€è‚¡æ°´è´¼åœ¨é™„è¿‘ä¸ºæ¶ï¼Œç¥¸å®³ç™¾å§“ï¼Œæ®‹æš´å¼‚å¸¸ï¼Œè¿˜å¸Œæœ›"..GetPlayerName(GetPlayer()).."å»å°†è¿™äº›æ¶äººé™¤æ‰ã€‚";
	action.m_ActionMsg = "ä¸ºæ°‘é™¤å®³æ˜¯æˆ‘è¾ˆæœ¬è‰²ï¼Œå°å¯å»å»å°±å›ã€‚";
	return action;
end

function Task_00000627_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ çœŸæ˜¯èº«æ‰‹ä¸å‡¡å•Šã€‚ ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000627_step_table = {
		[1] = Task_00000627_step_01,
		[10] = Task_00000627_step_10,
		};

function Task_00000627_step(step)
	if Task_00000627_step_table[step] ~= nil then
		return Task_00000627_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000627_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000627() then
		return false;
	end
	if not task:AcceptTask(627) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000627_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(627) then
		return false;
	end


	player:AddExp(3000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000627_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(627);
end
