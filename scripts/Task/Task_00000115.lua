--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000115()
	local player = GetPlayer();
	if player:GetLev() < 60 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(115) or task:HasCompletedTask(115) or task:HasSubmitedTask(115) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(114) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(114) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(114) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000115()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 60 then
		return false;
	end
	if task:HasAcceptedTask(115) or task:HasCompletedTask(115) or task:HasSubmitedTask(115) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(114) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(114) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(114) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000115()
	if GetPlayer():GetTaskMgr():HasCompletedTask(115) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000115(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(115) == npcId and Task_Accept_00000115 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 115
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ°´æ¶§æœ±æœ";
	elseif task:GetTaskSubmitNpc(115) == npcId then
		if Task_Submit_00000115() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 115
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ°´æ¶§æœ±æœ";
		elseif task:HasAcceptedTask(115) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 115
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ°´æ¶§æœ±æœ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000115_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ çŸ¥é“ä¸ï¼Œç¦»å’±ä»¬é©¬ç†Šæ´ä¸è¿œçš„åœ°æ–¹æœ‰ä¸€å¤„å­¤å³°ï¼Œå­¤å³°é‡Œçš„ä¸€å¤„æ°´æ¶§ä¸­ç”Ÿé•¿ç€ä¸€ç§æœ±æœï¼Œä¹ƒæ˜¯å¤©åœ°å¥‡çï¼Œå¯¹ä¿®é“ä¹‹äººå¤§æœ‰è£¨ç›Šï¼Œåªæ˜¯é‚£é‡Œè¢«ä¸€åªæœ¨é­ƒå æ®ï¼Œé‚£æœ±æœè™½å¥½å´æ˜¯ä¸å¥½è½»æ˜“æ‘˜å–ï¼Œä½ çœ‹â€¦â€¦";
	action.m_ActionMsg = "æˆ‘çœ‹å‡ºæ¥äº†ï¼Œä½ çš„å“ˆå–‡å­éƒ½æµåˆ°åœ°ä¸Šäº†ã€‚";
	return action;
end

function Task_00000115_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™ä¸‹å‘è¾¾äº†ï¼Œä»¥åå¯ä»¥å¤©å¤©åƒæœ±æœäº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000115_step_table = {
		[1] = Task_00000115_step_01,
		[10] = Task_00000115_step_10,
		};

function Task_00000115_step(step)
	if Task_00000115_step_table[step] ~= nil then
		return Task_00000115_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000115_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000115() then
		return false;
	end
	if not task:AcceptTask(115) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000115_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(115) then
		return false;
	end


	player:AddExp(34000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000115_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(115);
end
