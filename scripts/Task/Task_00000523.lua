--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000523()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(523) or task:HasCompletedTask(523) or task:HasSubmitedTask(523) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000523()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(523) or task:HasCompletedTask(523) or task:HasSubmitedTask(523) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000523()
	if GetPlayer():GetTaskMgr():HasCompletedTask(523) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000523(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(523) == npcId and Task_Accept_00000523 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 523
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "åŒ–å¤–å¦–é“";
	elseif task:GetTaskSubmitNpc(523) == npcId then
		if Task_Submit_00000523() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 523
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "åŒ–å¤–å¦–é“";
		elseif task:HasAcceptedTask(523) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 523
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "åŒ–å¤–å¦–é“";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000523_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é•¿æ²™éƒŠå¤–æœ‰ä¸€äº›æ¶äººï¼Œå—äº†é­”æ•™ä¸­äººçš„è›Šæƒ‘ï¼Œåœ¨é•¿æ²™ä¸ºæ¶ï¼Œä»–ä»¬è¿˜è¯·æ¥äº†åŒ–å¤–çš„å¦–é“ä½¿ç”¨é‚ªæ³•ï¼Œ"..GetPlayerName(GetPlayer()).."æˆ‘çœ‹å°±æ´¾ä½ å»ä¸€ç¨‹ï¼Œé“²é™¤äº†è¿™äº›æ¶äººã€‚";
	action.m_ActionMsg = "å‡ ä¸ªæ¶äººä¸åœ¨è¯ä¸‹ï¼Œæˆ‘å»å»å°±å›ã€‚";
	return action;
end

function Task_00000523_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ æœç„¶æ˜¯èº«æ‰‹ä¸å‡¡ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000523_step_table = {
		[1] = Task_00000523_step_01,
		[10] = Task_00000523_step_10,
		};

function Task_00000523_step(step)
	if Task_00000523_step_table[step] ~= nil then
		return Task_00000523_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000523_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000523() then
		return false;
	end
	if not task:AcceptTask(523) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000523_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(523) then
		return false;
	end


	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000523_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(523);
end
