--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000182()
	local player = GetPlayer();
	if player:GetLev() < 85 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(182) or task:HasCompletedTask(182) or task:HasSubmitedTask(182) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(181) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(181) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(181) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000182()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 85 then
		return false;
	end
	if task:HasAcceptedTask(182) or task:HasCompletedTask(182) or task:HasSubmitedTask(182) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(181) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(181) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(181) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000182()
	if GetPlayer():GetTaskMgr():HasCompletedTask(182) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000182(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(182) == npcId and Task_Accept_00000182 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 182
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ­¦å½“å›å¾’";
	elseif task:GetTaskSubmitNpc(182) == npcId then
		if Task_Submit_00000182() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 182
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ­¦å½“å›å¾’";
		elseif task:HasAcceptedTask(182) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 182
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ­¦å½“å›å¾’";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000182_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™æç´ç”Ÿä¹Ÿæ˜¯æˆ‘çš„åŒé—¨å¸ˆå…„å¼Ÿï¼Œä»–ç›®å‰å°±åœ¨è½èŠ±æ´å¤„ï¼Œæ˜¯æˆ‘ä»¬æ­¦å½“çš„è€»è¾±ï¼Œå°‘ä¾ å»è§£å†³ä»–å§ã€‚";
	action.m_ActionMsg = "æ˜¯å—ï¼Œæˆ‘è¿™å°±å»é™¤æ‰è¿™ä¸ªè´¥ç±»ã€‚";
	return action;
end

function Task_00000182_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ çœŸæ˜¯ä¿®ä¸ºæ·±åšï¼Œå¤©èµ‹å¼‚ç¦€å•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000182_step_table = {
		[1] = Task_00000182_step_01,
		[10] = Task_00000182_step_10,
		};

function Task_00000182_step(step)
	if Task_00000182_step_table[step] ~= nil then
		return Task_00000182_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000182_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000182() then
		return false;
	end
	if not task:AcceptTask(182) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000182_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(182) then
		return false;
	end


	player:AddExp(150000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000182_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(182);
end
