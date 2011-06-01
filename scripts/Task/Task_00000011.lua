--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000011()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(11) or task:HasCompletedTask(11) or task:HasSubmitedTask(11) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(10) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(10) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000011()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(11) or task:HasCompletedTask(11) or task:HasSubmitedTask(11) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(10) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(10) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000011()
	if GetPlayer():GetTaskMgr():HasCompletedTask(11) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000011(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(11) == npcId and Task_Accept_00000011 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 11
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "é†‰é“äºº";
	elseif task:GetTaskSubmitNpc(11) == npcId then
		if Task_Submit_00000011() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 11
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "é†‰é“äºº";
		elseif task:HasAcceptedTask(11) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 11
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "é†‰é“äºº";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000011_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "â€¦â€¦â€¦â€¦";
	action.m_ActionMsg = "è¿™äººæ€ä¹ˆæ¨ªå§åœ¨è·¯ä¸Šã€‚";
	return action;
end

function Task_00000011_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 3;
	action.m_NpcMsg = "â€¦â€¦â€¦â€¦ä½•ä»¥è§£å¿§ï¼Œå”¯æœ‰æœåº·ï¼";
	action.m_ActionMsg = "çœ‹ä»–æ ·å­ï¼Œåƒæ˜¯ä¸ªé£å°˜å¼‚äººï¼Œä¸å¦¨å¸®ä»–ä¸€æŠŠã€‚";
	return action;
end

function Task_00000011_step_03()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 4;
	action.m_NpcMsg = "é…’â€¦â€¦ç»™æˆ‘å¼„ç‚¹é…’æ¥â€¦";
	action.m_ActionMsg = "å‰è¾ˆï¼Œæ­¤å¤„æ— é…’ï¼Œä¸å¦¨æˆ‘å¼•ä½ å»å‰é¢é…’æ¥¼å¦‚ä½•ï¼Ÿ";
	return action;
end

function Task_00000011_step_04()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é…’â€¦â€¦ç»™æˆ‘å¼„é…’æ¥â€¦â€¦";
	action.m_ActionMsg = "è¿˜æ˜¯å…ˆå»å¸®ä»–å»ä¹°ç‚¹é…’æ¥å§ã€‚";
	return action;
end

function Task_00000011_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™ä½å®¢å®˜æ˜¯è¦ä½åº—å‘¢è¿˜æ˜¯é¥®é…’ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000011_step_table = {
		[1] = Task_00000011_step_01,
		[2] = Task_00000011_step_02,
		[3] = Task_00000011_step_03,
		[4] = Task_00000011_step_04,
		[10] = Task_00000011_step_10,
		};

function Task_00000011_step(step)
	if Task_00000011_step_table[step] ~= nil then
		return Task_00000011_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000011_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000011() then
		return false;
	end
	if not task:AcceptTask(11) then
		return false;
	end
	task:AddTaskStep(11);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000011_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(11) then
		return false;
	end


	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000011_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(11);
end
