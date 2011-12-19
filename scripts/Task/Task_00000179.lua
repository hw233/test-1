--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000179()
	local player = GetPlayer();
	if player:GetLev() < 85 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(179) or task:HasCompletedTask(179) or task:HasSubmitedTask(179) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(178) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(178) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(178) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000179()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 85 then
		return false;
	end
	if task:HasAcceptedTask(179) or task:HasCompletedTask(179) or task:HasSubmitedTask(179) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(178) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(178) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(178) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000179()
	if GetPlayer():GetTaskMgr():HasCompletedTask(179) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000179(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(179) == npcId and Task_Accept_00000179 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 179
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç¥–å¸ˆçµä½";
	elseif task:GetTaskSubmitNpc(179) == npcId then
		if Task_Submit_00000179() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 179
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç¥–å¸ˆçµä½";
		elseif task:HasAcceptedTask(179) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 179
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç¥–å¸ˆçµä½";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000179_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å—¯ï¼Œå¤šè°¢å°‘ä¾ å¸®å¿™ï¼Œæ­¦å½“æ–°é­å¤§å˜ï¼Œè¿˜é¡»å€ŸåŠ©å°‘ä¾ ä¹‹åŠ›ï¼Œè™½ç„¶æ˜¯æ­¦å½“å†…éƒ¨ä¹‹äº‹ï¼Œåªè¦å°‘ä¾ é€šè¿‡ä¸€äº›è€ƒéªŒï¼Œä¹Ÿä¸ç®—å¤–äººï¼Œä½ å»ç¥­æ‹œä¸€ä¸‹ç¥–å¸ˆçµä½ï¼Œç¥­æ‹œé“œäººé˜µå§ã€‚";
	action.m_ActionMsg = "æ˜¯å—ï¼Œæˆ‘è¿™å°±å»çœ‹çœ‹ã€‚";
	return action;
end

function Task_00000179_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ çœŸæ˜¯ä¿®ä¸ºæ·±åšï¼Œå¤©èµ‹å¼‚ç¦€å•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000179_step_table = {
		[1] = Task_00000179_step_01,
		[10] = Task_00000179_step_10,
		};

function Task_00000179_step(step)
	if Task_00000179_step_table[step] ~= nil then
		return Task_00000179_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000179_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000179() then
		return false;
	end
	if not task:AcceptTask(179) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000179_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(179) then
		return false;
	end


	player:AddExp(150000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000179_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(179);
end
