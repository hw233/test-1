--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000160()
	local player = GetPlayer();
	if player:GetLev() < 75 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(160) or task:HasCompletedTask(160) or task:HasSubmitedTask(160) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(159) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(159) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(159) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000160()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 75 then
		return false;
	end
	if task:HasAcceptedTask(160) or task:HasCompletedTask(160) or task:HasSubmitedTask(160) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(159) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(159) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(159) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000160()
	if GetPlayer():GetTaskMgr():HasCompletedTask(160) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000160(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(160) == npcId and Task_Accept_00000160 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 160
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å´‡æ˜å²›æ¶äºº";
	elseif task:GetTaskSubmitNpc(160) == npcId then
		if Task_Submit_00000160() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 160
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å´‡æ˜å²›æ¶äºº";
		elseif task:HasAcceptedTask(160) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 160
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å´‡æ˜å²›æ¶äºº";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000160_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å³¨çœ‰æ´¾çš„å‡ ä½å°‘ä¾ æ–¹æ‰åŠ©æˆ‘æ¸¡åŠ«ï¼Œå¦‚ä»Šå¤©åŠ«å·²æ¸¡ï¼Œä»–ä»¬ä¹Ÿçº·çº·ç¦»å»ï¼Œä¸è¿‡æ–¹æ‰è§æè‹±ç¼ç¥è‰²ç„¦æ€¥ï¼Œä¼¼ä¹æ˜¯å¥¹çš„çˆ±å® ä½›å¥´é‡æ•Œï¼Œå¾€å´‡æ˜å²›æ–¹å‘å»äº†ã€‚é‚£åº§å²›ä¸Šæœ‰ä¸€ä¸ªå¦–å¦‡å”¤ä½œè’²å¦™å¦™ï¼Œä¸å¦‚ä½ èµ¶å»ä¸è‹±ç¼è”æ‰‹é™¤æ‰æ­¤æ¶ã€‚";
	action.m_ActionMsg = "å—¯ï¼Œæˆ‘çŸ¥é“äº†ã€‚";
	return action;
end

function Task_00000160_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™ç­‰é‚ªæ´¾äººç‰©å®šè¦æ—©æ—©æ¶ˆç­ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000160_step_table = {
		[1] = Task_00000160_step_01,
		[10] = Task_00000160_step_10,
		};

function Task_00000160_step(step)
	if Task_00000160_step_table[step] ~= nil then
		return Task_00000160_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000160_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000160() then
		return false;
	end
	if not task:AcceptTask(160) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000160_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(160) then
		return false;
	end


	player:AddExp(100000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000160_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(160);
end
