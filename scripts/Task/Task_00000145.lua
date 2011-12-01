--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000145()
	local player = GetPlayer();
	if player:GetLev() < 65 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(145) or task:HasCompletedTask(145) or task:HasSubmitedTask(145) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000145()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 65 then
		return false;
	end
	if task:HasAcceptedTask(145) or task:HasCompletedTask(145) or task:HasSubmitedTask(145) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000145()
	if GetPlayer():GetTaskMgr():HasCompletedTask(145) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000145(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(145) == npcId and Task_Accept_00000145 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 145
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¦–é“ç¥é¸Ÿ";
	elseif task:GetTaskSubmitNpc(145) == npcId then
		if Task_Submit_00000145() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 145
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¦–é“ç¥é¸Ÿ";
		elseif task:HasAcceptedTask(145) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 145
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¦–é“ç¥é¸Ÿ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000145_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™ä¸‰å°¸å¢“æ—©åœ¨é»„å¸æ—¶ä»£å°±å·²å­˜åœ¨ï¼Œå¢“ä¸­çš„ä¸¤ä¸ªåƒµå°¸å¦–æœ¯éå¸¸å‰å®³ï¼Œåæ¥ä»–ä»¬å·ç›—äº†è½©è¾•ç¥å¢“ä¸­çš„æ³•å®ï¼Œå¹¶å‹¾ç»“äº†ä¸€ä¸ªå¦–é“å«é’Ÿæ•¢åœ¨åºå±±ä¸ºæ¶ï¼Œé‚£å¦–é“é’Ÿæ•¢æ‰‹ä¸‹ä¸€ç›´ç¥é¸Ÿå¤©ç”Ÿå‡¶ç¦½ï¼Œå°‘ä¾ ä½ è¦å°å¿ƒå¯¹ä»˜æ‰æ˜¯ã€‚";
	action.m_ActionMsg = "ä¸€åªæ‰æ¯›ç•œç”Ÿè€Œå·²ï¼Œå¾…æˆ‘å»æ¶ˆç­å®ƒã€‚";
	return action;
end

function Task_00000145_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ï¼Œå°‘ä¾ çœŸæ˜¯å‰‘æœ¯é«˜å¼ºï¼Œä¸€èº«æ­£æ°”å•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000145_step_table = {
		[1] = Task_00000145_step_01,
		[10] = Task_00000145_step_10,
		};

function Task_00000145_step(step)
	if Task_00000145_step_table[step] ~= nil then
		return Task_00000145_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000145_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000145() then
		return false;
	end
	if not task:AcceptTask(145) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000145_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(145) then
		return false;
	end


	player:AddExp(80000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000145_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(145);
end
