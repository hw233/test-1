--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000602()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(602) or task:HasCompletedTask(602) or task:HasSubmitedTask(602) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000602()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(602) or task:HasCompletedTask(602) or task:HasSubmitedTask(602) then
		return false;
	end
	if player:ColorTaskOutOf(1) then
		return false
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000602()
	if GetPlayer():GetTaskMgr():HasCompletedTask(602) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000602(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(602) == npcId and Task_Accept_00000602 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 602
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "è›‡å¦–ä¹‹ç¥¸";
	elseif task:GetTaskSubmitNpc(602) == npcId then
		if Task_Submit_00000602() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 602
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "è›‡å¦–ä¹‹ç¥¸";
		elseif task:HasAcceptedTask(602) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 602
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "è›‡å¦–ä¹‹ç¥¸";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000602_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆéƒ½éƒŠå¤–çš„äº‘çµå±±æœ‰ä¸€åªè›‡å¦–ç›˜è¸ï¼Œç»å¸¸ç¥¸å®³è¿‡å¾€è¡Œäººã€‚è¿™åªè›‡å¦–æƒ¯äºå–·æ¶‚å‰§æ¯’ï¼Œæˆ‘æ‰‹ä¸‹çš„è¡™å½¹æ•å¿«æ‹¿å®ƒæ¯«æ— åŠæ³•ï¼Œè¿˜å¸Œæœ›"..GetPlayerName(GetPlayer()).."å»å°†è¿™åªå¦–ç‰©æ¶ˆç­ã€‚";
	action.m_ActionMsg = "ä¸ºæ°‘é™¤å®³æ˜¯æˆ‘è¾ˆæœ¬è‰²ï¼Œå¼Ÿå­å»å»å°±å›ã€‚";
	return action;
end

function Task_00000602_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ çœŸæ˜¯èº«æ‰‹ä¸å‡¡å•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000602_step_table = {
		[1] = Task_00000602_step_01,
		[10] = Task_00000602_step_10,
		};

function Task_00000602_step(step)
	if Task_00000602_step_table[step] ~= nil then
		return Task_00000602_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000602_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000602() then
		return false;
	end
	if not task:AcceptTask(602) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000602_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(602) then
		return false;
	end


	player:AddExp(2222);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000602_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(602);
end
