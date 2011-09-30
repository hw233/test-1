--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000571()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(571) or task:HasCompletedTask(571) or task:HasSubmitedTask(571) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000571()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(571) or task:HasCompletedTask(571) or task:HasSubmitedTask(571) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000571()
	if GetPlayer():GetTaskMgr():HasCompletedTask(571) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000571(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(571) == npcId and Task_Accept_00000571 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 571
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å•æ‘æ‰“æ‰‹";
	elseif task:GetTaskSubmitNpc(571) == npcId then
		if Task_Submit_00000571() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 571
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å•æ‘æ‰“æ‰‹";
		elseif task:HasAcceptedTask(571) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 571
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å•æ‘æ‰“æ‰‹";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000571_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é•¿æ²™éƒŠå¤–æœ‰ä¸€äº›æ¶äººï¼Œå—äº†é­”æ•™ä¸­äººçš„è›Šæƒ‘ï¼Œåœ¨é•¿æ²™ä¸ºæ¶ï¼Œ"..GetPlayerName(GetPlayer()).."ä½ è¿™å°±å»é•¿æ²™éƒŠå¤–çœ‹çœ‹ï¼Œå¦‚æœé‡åˆ°å°±é¡ºæ‰‹é“²é™¤äº†è¿™äº›æ¶äººã€‚";
	action.m_ActionMsg = "å¼Ÿå­éµå‘½ï¼Œæˆ‘å»å»å°±å›ã€‚";
	return action;
end

function Task_00000571_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ çœŸæ˜¯èº«æ‰‹ä¸å‡¡å•Šã€‚ ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000571_step_table = {
		[1] = Task_00000571_step_01,
		[10] = Task_00000571_step_10,
		};

function Task_00000571_step(step)
	if Task_00000571_step_table[step] ~= nil then
		return Task_00000571_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000571_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000571() then
		return false;
	end
	if not task:AcceptTask(571) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000571_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(571) then
		return false;
	end


	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000571_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(571);
end
