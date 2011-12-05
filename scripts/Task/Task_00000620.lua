--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000620()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(620) or task:HasCompletedTask(620) or task:HasSubmitedTask(620) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000620()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(620) or task:HasCompletedTask(620) or task:HasSubmitedTask(620) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000620()
	if GetPlayer():GetTaskMgr():HasCompletedTask(620) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000620(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(620) == npcId and Task_Accept_00000620 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 620
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ­£é“å¼ƒå¾’";
	elseif task:GetTaskSubmitNpc(620) == npcId then
		if Task_Submit_00000620() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 620
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ­£é“å¼ƒå¾’";
		elseif task:HasAcceptedTask(620) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 620
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ­£é“å¼ƒå¾’";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000620_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é•¿æ²™éƒŠå¤–æœ‰ä¸€ä¸ªæ¶äººï¼Œä¹‹å‰æ˜¯ä¸‡é‡Œé£è™¹å†¬å…ƒå¥‡çš„å¼Ÿå­ï¼Œå› ä¸ºå¿ƒæœ¯ä¸æ­£è¢«é€å‡ºå¸ˆé—¨ï¼Œå¦‚ä»Šåœ¨é•¿æ²™éƒŠå¤–ä¸ºæ¶ï¼Œ"..GetPlayerName(GetPlayer()).."ä½ è¿™å°±å»å°†è¿™ä¸ªæ¶äººé™¤æ‰ã€‚";
	action.m_ActionMsg = "å¥½å§ï¼Œæˆ‘å»å»å°±å›ã€‚";
	return action;
end

function Task_00000620_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ æœç„¶æ˜¯èº«æ‰‹ä¸å‡¡ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000620_step_table = {
		[1] = Task_00000620_step_01,
		[10] = Task_00000620_step_10,
		};

function Task_00000620_step(step)
	if Task_00000620_step_table[step] ~= nil then
		return Task_00000620_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000620_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000620() then
		return false;
	end
	if not task:AcceptTask(620) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000620_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(620) then
		return false;
	end


	player:AddExp(3000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000620_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(620);
end
