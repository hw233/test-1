--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000119()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(119) or task:HasCompletedTask(119) or task:HasSubmitedTask(119) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(118) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(118) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(118) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000119()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(119) or task:HasCompletedTask(119) or task:HasSubmitedTask(119) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(118) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(118) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(118) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000119()
	if GetPlayer():GetTaskMgr():HasCompletedTask(119) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000119(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(119) == npcId and Task_Accept_00000119 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 119
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å†°èš•";
	elseif task:GetTaskSubmitNpc(119) == npcId then
		if Task_Submit_00000119() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 119
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å†°èš•";
		elseif task:HasAcceptedTask(119) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 119
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å†°èš•";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000119_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è½è‹å±±è¿™å¤„å†°ç©´çœŸæ˜¯ä¸–æ‰€ç½•è§ï¼Œåœ¨å†°ç©´æ·±å¤„å­•è‚²ç€ä¸€åªæ•°ç™¾å¹´é“è¡Œçš„å†°èš•ï¼Œé˜´å¯’æ— æ¯”ï¼Œè‹¥è¢«é‚£äº›é‚ªé“äººå£«è·å¾—ï¼Œç»ƒæˆé‚ªåŠŸå°±æ˜¯ä¸€åœºç¾éš¾äº†ï¼Œ"..GetPlayerName(GetPlayer()).."ä¸å¦‚ä½ å»å†°ç©´å†…å°†å®ƒé™æœã€‚";
	action.m_ActionMsg = "å—¯ï¼Œæˆ‘è¿™å°±å»ä¼šä¼šè¿™åªå†°èš•ã€‚";
	return action;
end

function Task_00000119_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™ä¸ªå†°èš•çœŸæ˜¯å¤©åœ°å¥‡çã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000119_step_table = {
		[1] = Task_00000119_step_01,
		[10] = Task_00000119_step_10,
		};

function Task_00000119_step(step)
	if Task_00000119_step_table[step] ~= nil then
		return Task_00000119_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000119_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000119() then
		return false;
	end
	if not task:AcceptTask(119) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000119_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(119) then
		return false;
	end


	player:AddExp(2222);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000119_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(119);
end
