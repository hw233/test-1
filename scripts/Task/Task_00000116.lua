--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000116()
	local player = GetPlayer();
	if player:GetLev() < 60 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(116) or task:HasCompletedTask(116) or task:HasSubmitedTask(116) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(115) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(115) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(115) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000116()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 60 then
		return false;
	end
	if task:HasAcceptedTask(116) or task:HasCompletedTask(116) or task:HasSubmitedTask(116) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(115) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(115) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(115) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000116()
	if GetPlayer():GetTaskMgr():HasCompletedTask(116) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000116(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(116) == npcId and Task_Accept_00000116 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 116
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "èµµç¥æ®¿";
	elseif task:GetTaskSubmitNpc(116) == npcId then
		if Task_Submit_00000116() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 116
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "èµµç¥æ®¿";
		elseif task:HasAcceptedTask(116) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 116
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "èµµç¥æ®¿";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000116_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è½è‹å±±æ–¹åœ†å‡ ç™¾é‡Œï¼Œæˆ‘ä¹Ÿæ²¡å…¨éƒ¨éƒ½è·‘è¿‡ï¼Œæ›¾è§åˆ°ä¸œé¢ä¸è¿œå¤„æœ‰ä¸ªèµµç¥æ®¿ï¼Œç›˜è¸ç€äº›åƒµå°¸ï¼Œæœ‰å¥½å¤šå¹´å¤´äº†ï¼Œ"..GetPlayerName(GetPlayer()).."ä¸çŸ¥é“ä½ æœ‰æ²¡èƒ†é‡é™¤æ‰è¿™äº›æ¶å¿ƒçš„å®¶ä¼™ã€‚";
	action.m_ActionMsg = "è¿™è¿˜ä¸æ˜¯å°èœä¸€ç¢Ÿã€‚";
	return action;
end

function Task_00000116_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é‚£äº›æµ‘èº«è…çƒ‚çš„å®¶ä¼™ä½ ä¹Ÿèƒ½å¯¹ä»˜ï¼Œæˆ‘çœŸçš„æœäº†ä½ äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000116_step_table = {
		[1] = Task_00000116_step_01,
		[10] = Task_00000116_step_10,
		};

function Task_00000116_step(step)
	if Task_00000116_step_table[step] ~= nil then
		return Task_00000116_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000116_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000116() then
		return false;
	end
	if not task:AcceptTask(116) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000116_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(116) then
		return false;
	end


	player:AddExp(36000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000116_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(116);
end
