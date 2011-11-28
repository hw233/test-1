--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000159()
	local player = GetPlayer();
	if player:GetLev() < 75 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(159) or task:HasCompletedTask(159) or task:HasSubmitedTask(159) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(158) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(158) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(158) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000159()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 75 then
		return false;
	end
	if task:HasAcceptedTask(159) or task:HasCompletedTask(159) or task:HasSubmitedTask(159) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(158) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(158) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(158) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000159()
	if GetPlayer():GetTaskMgr():HasCompletedTask(159) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000159(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(159) == npcId and Task_Accept_00000159 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 159
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å®ç›¸å¤«äºº";
	elseif task:GetTaskSubmitNpc(159) == npcId then
		if Task_Submit_00000159() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 159
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å®ç›¸å¤«äºº";
		elseif task:HasAcceptedTask(159) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 159
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å®ç›¸å¤«äºº";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000159_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘æ˜”å¹´ç‚¼åˆ¶çš„ç™½çœ‰é’ˆæ€æˆ®å¤ªé‡ï¼Œå‘½ä¸­æ³¨å®šä¼šæœ‰è¿™ä¹ˆä¸€åŠ«ï¼Œè™½ç„¶ç™¾å¹´æ¥æˆ‘é™å¿ƒä¿®é“ï¼Œä¸å†é€ æ€å­½ï¼Œä¸è¿‡è¿™å¤©åŠ«ä»ç„¶å¦‚æœŸè€Œè‡³ã€‚å¦‚ä»Šæˆ‘ä¸“å¿ƒæ¸¡åŠ«ï¼Œä¸èƒ½åˆ†å¿ƒï¼Œè¿˜æœ›å°‘ä¾ å¸®åŠ©é™¤æ‰é‚£é„¢ä»€ï¼Œæ­¤äººä¿®ç‚¼çš„è¯¡å¼‚é­”å®«ï¼Œæ“çºµåƒµå°¸ï¼Œä½ è¦å°å¿ƒå¯¹ä»˜æ‰æ˜¯ã€‚";
	action.m_ActionMsg = "å—¯ï¼Œæˆ‘çŸ¥é“äº†ã€‚";
	return action;
end

function Task_00000159_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¤šè°¢è¯¸ä½é“å‹ç›¸åŠ©ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000159_step_table = {
		[1] = Task_00000159_step_01,
		[10] = Task_00000159_step_10,
		};

function Task_00000159_step(step)
	if Task_00000159_step_table[step] ~= nil then
		return Task_00000159_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000159_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000159() then
		return false;
	end
	if not task:AcceptTask(159) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000159_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(159) then
		return false;
	end


	player:AddExp(100000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000159_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(159);
end
