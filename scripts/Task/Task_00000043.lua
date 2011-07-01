--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000043()
	local player = GetPlayer();
	if player:GetLev() < 28 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(43) or task:HasCompletedTask(43) or task:HasSubmitedTask(43) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(42) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(42) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000043()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 28 then
		return false;
	end
	if task:HasAcceptedTask(43) or task:HasCompletedTask(43) or task:HasSubmitedTask(43) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(42) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(42) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000043()
	if GetPlayer():GetTaskMgr():HasCompletedTask(43) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000043(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(43) == npcId and Task_Accept_00000043 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 43
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "çŸ¥å®¢äº†ä¸€";
	elseif task:GetTaskSubmitNpc(43) == npcId then
		if Task_Submit_00000043() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 43
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "çŸ¥å®¢äº†ä¸€";
		elseif task:HasAcceptedTask(43) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 43
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "çŸ¥å®¢äº†ä¸€";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000043_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é˜¿å¼¥é™€ä½›ï¼Œå°‘ä¾ æ€•æ˜¯æ¥æ™šäº†ï¼Œé‚£å¸®ä¹¦ç”Ÿå½“æ™šå°±é­äº†æ¯’æ‰‹ï¼Œä¸è¿‡è¿˜å‰©ä¸€ä¸ªå«å‘¨äº‘ä»çš„è¢«å…³åœ¨åœ°ç‰¢å†…ã€‚ä¸è¿‡æ…ˆäº‘å¯ºè¿‘æ—¥æ¥çœ‹ç®¡ç”šä¸¥ï¼Œå¹¿åœºå†…å¸¸æœ‰åƒ§å…µå·¡é€»ï¼Œè‹¥æƒ³å·å…¥åœ°ç‰¢ï¼Œè¿˜å¾—å…ˆé™¤æ‰è¿™äº›è€³ç›®æ‰è¡Œã€‚";
	action.m_ActionMsg = "æˆ‘ä¸€å®šåšçš„ç¥ä¸çŸ¥é¬¼ä¸è§‰ã€‚";
	return action;
end

function Task_00000043_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ çœŸæ˜¯å¥½èº«æ‰‹ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000043_step_table = {
		[1] = Task_00000043_step_01,
		[10] = Task_00000043_step_10,
		};

function Task_00000043_step(step)
	if Task_00000043_step_table[step] ~= nil then
		return Task_00000043_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000043_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000043() then
		return false;
	end
	if not task:AcceptTask(43) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000043_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(43) then
		return false;
	end


	player:AddExp(1111);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000043_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(43);
end
