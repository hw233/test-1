--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090112()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90112) or task:HasCompletedTask(90112) or task:HasSubmitedTask(90112) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90111) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90111) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090112()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90112) or task:HasCompletedTask(90112) or task:HasSubmitedTask(90112) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90111) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90111) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090112()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90112) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090112(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90112) == npcId and Task_Accept_00090112 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90112
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "èŒå®ˆé˜µè¥";
	elseif task:GetTaskSubmitNpc(90112) == npcId then
		if Task_Submit_00090112() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90112
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "èŒå®ˆé˜µè¥";
		elseif task:HasAcceptedTask(90112) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90112
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "èŒå®ˆé˜µè¥";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090112_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "åŸæ¥ä½ æ˜¯æ¥åŠå°†çš„ã€‚";
	action.m_ActionMsg = "æ­£æ˜¯ï¼Œåœ¨ä¸‹å¥‰å‹’å‹¤å°†å†›å‘½ï¼Œç‰¹æ¥é‚€è¯·å•å…ˆç”ŸååŠ©ä½œæˆ˜ã€‚";
	return action;
end

function Task_00090112_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 3;
	action.m_NpcMsg = "å‘µå‘µï¼Œä»–è¿˜æŒºä¼šæƒ³çš„ã€‚å¾ˆæŠ±æ­‰ï¼Œæˆ‘å¿…é¡»é•‡å®ˆæ­¤å¤„ï¼Œå“ªå„¿ä¹Ÿä¸èƒ½å»ï¼Œæˆ‘ä¹Ÿä¸æƒ³å»ã€‚";
	action.m_ActionMsg = "è¿™å«åœ¨ä¸‹å¦‚ä½•æ˜¯å¥½ã€‚";
	return action;
end

function Task_00090112_step_03()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ æ›¿æˆ‘å»å›äº†é‚£å‹’å‹¤å§ã€‚";
	action.m_ActionMsg = "çœ‹æ¥å‹’å°†å†›è¦å¤±æœ›äº†ã€‚";
	return action;
end

function Task_00090112_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä»–æœçœŸæ˜¯ä¸æ„¿æ¥ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090112_step_table = {
		[1] = Task_00090112_step_01,
		[2] = Task_00090112_step_02,
		[3] = Task_00090112_step_03,
		[10] = Task_00090112_step_10,
		};

function Task_00090112_step(step)
	if Task_00090112_step_table[step] ~= nil then
		return Task_00090112_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090112_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090112() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90112) then
		return false;
	end
	task:AddTaskStep(90112);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090112_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90112) then
		return false;
	end


	player:AddExp(600000);
	player:getCoin(308000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090112_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90112);
end
