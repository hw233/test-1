--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090124()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90124) or task:HasCompletedTask(90124) or task:HasSubmitedTask(90124) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90123) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90123) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090124()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90124) or task:HasCompletedTask(90124) or task:HasSubmitedTask(90124) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90123) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90123) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090124()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90124) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090124(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90124) == npcId and Task_Accept_00090124 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90124
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¯æ¶çš„ç‹¼äºº";
	elseif task:GetTaskSubmitNpc(90124) == npcId then
		if Task_Submit_00090124() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90124
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¯æ¶çš„ç‹¼äºº";
		elseif task:HasAcceptedTask(90124) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90124
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¯æ¶çš„ç‹¼äºº";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090124_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "æˆ‘ä»¬å¿…é¡»æ•™è®­é‚£äº›æ€ªç‰©ã€‚";
	action.m_ActionMsg = "å…ˆæ¶ˆæ¶ˆæ°”ï¼Œæˆ‘ä»¬å¿…é¡»ä»é•¿è®¡è®®ã€‚";
	return action;
end

function Task_00090124_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä¸ç”¨æƒ³é‚£ä¹ˆè¿œï¼Œä½ å»å¸®æˆ‘æ€æ¯æœ¨ç‹¼äººï¼Œæ–¹èƒ½è§£æˆ‘å¿ƒå¤´ä¹‹æ¨ã€‚";
	action.m_ActionMsg = "è¿™ä¸ªæ²¡é—®é¢˜ï¼Œæˆ‘ä¹æ„é“²å¥¸é™¤æ¶ã€‚";
	return action;
end

function Task_00090124_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "çœ‹ä»–ä»¬è¿˜æ•¢é€ æ¬¡ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090124_step_table = {
		[1] = Task_00090124_step_01,
		[2] = Task_00090124_step_02,
		[10] = Task_00090124_step_10,
		};

function Task_00090124_step(step)
	if Task_00090124_step_table[step] ~= nil then
		return Task_00090124_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090124_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090124() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90124) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090124_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90124) then
		return false;
	end


	player:AddExp(700000);
	player:getCoin(316000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090124_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90124);
end
