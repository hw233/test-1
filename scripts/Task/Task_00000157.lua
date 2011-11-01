--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000157()
	local player = GetPlayer();
	if player:GetLev() < 71 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(157) or task:HasCompletedTask(157) or task:HasSubmitedTask(157) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000157()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 71 then
		return false;
	end
	if task:HasAcceptedTask(157) or task:HasCompletedTask(157) or task:HasSubmitedTask(157) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000157()
	if GetPlayer():GetTaskMgr():HasCompletedTask(157) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000157(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(157) == npcId and Task_Accept_00000157 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 157
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç´«äº‘ä¹‹ä¹±";
	elseif task:GetTaskSubmitNpc(157) == npcId then
		if Task_Submit_00000157() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 157
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç´«äº‘ä¹‹ä¹±";
		elseif task:HasAcceptedTask(157) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 157
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç´«äº‘ä¹‹ä¹±";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000157_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç´«äº‘å®«çš„é‡‘é¡»å¥´åŸæ˜¯å¤©åœ°ä¹¾æ˜ç¦»ç«æ‰€åŒ–ï¼Œè™½ç„¶ä¸ç´«äº‘ä¸‰å¥³ä¸€èµ·ï¼Œä½†ä»ä¸€å¿ƒå‘å–„ï¼Œåªæ˜¯å› ä¸ºéœ€è¦å¤©ä¸€è´æ°´æ¥èä¼šåç¦»ï¼Œæ‰€ä»¥æ•¢ä¸ºå¥´ä»†éšåˆå‡¤ä¿®è¡Œï¼Œä¸è¿‡ä»–æ²¡ç”šæ¶è¿¹ï¼Œæ‰€ä»¥ä½ å¯æ‰‹ä¸‹ç•™æƒ…ï¼Œå°†ä»–å‡»è´¥å³å¯ã€‚";
	action.m_ActionMsg = "å—¯ï¼Œæˆ‘è¿™å°±å»ã€‚";
	return action;
end

function Task_00000157_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç´«äº‘å®«åŸæ˜¯ä»™äººåºœé‚¸ï¼Œä½†æ˜¯ç°åœ¨å´è¢«è¿™ç¾¤å¦–äººå æ®ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000157_step_table = {
		[1] = Task_00000157_step_01,
		[10] = Task_00000157_step_10,
		};

function Task_00000157_step(step)
	if Task_00000157_step_table[step] ~= nil then
		return Task_00000157_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000157_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000157() then
		return false;
	end
	if not task:AcceptTask(157) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000157_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(157) then
		return false;
	end


	player:AddExp(110000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000157_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(157);
end
