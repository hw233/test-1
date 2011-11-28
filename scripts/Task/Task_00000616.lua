--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000616()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(616) or task:HasCompletedTask(616) or task:HasSubmitedTask(616) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000616()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(616) or task:HasCompletedTask(616) or task:HasSubmitedTask(616) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000616()
	if GetPlayer():GetTaskMgr():HasCompletedTask(616) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000616(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(616) == npcId and Task_Accept_00000616 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 616
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å•æ‘æ‰“æ‰‹";
	elseif task:GetTaskSubmitNpc(616) == npcId then
		if Task_Submit_00000616() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 616
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å•æ‘æ‰“æ‰‹";
		elseif task:HasAcceptedTask(616) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 616
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
function Task_00000616_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é•¿æ²™éƒŠå¤–æœ‰ä¸€äº›æ¶äººï¼Œå—äº†é­”æ•™ä¸­äººçš„è›Šæƒ‘ï¼Œåœ¨é•¿æ²™ä¸ºæ¶ï¼Œ"..GetPlayerName(GetPlayer()).."ä½ è¿™å°±å»é•¿æ²™éƒŠå¤–çœ‹çœ‹ï¼Œå¦‚æœé‡åˆ°å°±é¡ºæ‰‹é“²é™¤äº†è¿™äº›æ¶äººã€‚";
	action.m_ActionMsg = "å‡ ä¸ªæ¶äººä¸åœ¨è¯ä¸‹ï¼Œæˆ‘å»å»å°±å›ã€‚";
	return action;
end

function Task_00000616_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ æœç„¶æ˜¯èº«æ‰‹ä¸å‡¡ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000616_step_table = {
		[1] = Task_00000616_step_01,
		[10] = Task_00000616_step_10,
		};

function Task_00000616_step(step)
	if Task_00000616_step_table[step] ~= nil then
		return Task_00000616_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000616_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000616() then
		return false;
	end
	if not task:AcceptTask(616) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000616_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(616) then
		return false;
	end


	player:AddExp(3000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000616_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(616);
end
