--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000169()
	local player = GetPlayer();
	if player:GetLev() < 80 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(169) or task:HasCompletedTask(169) or task:HasSubmitedTask(169) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(168) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(168) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(168) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000169()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 80 then
		return false;
	end
	if task:HasAcceptedTask(169) or task:HasCompletedTask(169) or task:HasSubmitedTask(169) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(168) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(168) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(168) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000169()
	if GetPlayer():GetTaskMgr():HasCompletedTask(169) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000169(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(169) == npcId and Task_Accept_00000169 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 169
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å†°ç†Š";
	elseif task:GetTaskSubmitNpc(169) == npcId then
		if Task_Submit_00000169() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 169
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å†°ç†Š";
		elseif task:HasAcceptedTask(169) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 169
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å†°ç†Š";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000169_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™é™·ç©ºå²›åœ°å¤„åŒ—æï¼Œå€’ä¹Ÿæœ‰ä¸€äº›å¥‡å¼‚çš„ç”Ÿç‰©ï¼Œä½ çœ‹é‚£ç§€ç¼åŸä¸Šæœ‰ä¸€äº›å†°ç†Šï¼Œæ¯›çš®é›ªç™½ï¼ŒåŠ›å¤§æ— ç©·ï¼Œç”¨æ¥åšè¿è¾“çš„ç•œåŠ›å¾ˆæ˜¯ä¸é”™ï¼Œå°å‹ä½ å»å¸®æˆ‘æŠ“ä¸€äº›æ¥ã€‚";
	action.m_ActionMsg = "è€ç¥–å©å’ï¼Œå¼Ÿå­è¿™å°±å»åŠã€‚";
	return action;
end

function Task_00000169_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å“ˆå“ˆï¼Œæ—©å°±å¬è¯´å°å‹ä½ åœ¨æ­£é‚ªä¹‹æˆ˜ä¸­å¤§æ”¾å¼‚å½©ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000169_step_table = {
		[1] = Task_00000169_step_01,
		[10] = Task_00000169_step_10,
		};

function Task_00000169_step(step)
	if Task_00000169_step_table[step] ~= nil then
		return Task_00000169_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000169_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000169() then
		return false;
	end
	if not task:AcceptTask(169) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000169_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(169) then
		return false;
	end


	player:AddExp(150000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000169_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(169);
end
