--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080602()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80602) or task:HasCompletedTask(80602) or task:HasSubmitedTask(80602) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80601) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80601) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00080602()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80602) or task:HasCompletedTask(80602) or task:HasSubmitedTask(80602) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80601) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80601) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00080602()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80602) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080602(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80602) == npcId and Task_Accept_00080602 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80602
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æŠ¢å¤ºçŸ³æ¡¥";
	elseif task:GetTaskSubmitNpc(80602) == npcId then
		if Task_Submit_00080602() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80602
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æŠ¢å¤ºçŸ³æ¡¥";
		elseif task:HasAcceptedTask(80602) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80602
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æŠ¢å¤ºçŸ³æ¡¥";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080602_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "æœç„¶åœ¨æˆ‘é¢„æ–™ä¹‹å†…ã€‚";
	action.m_ActionMsg = "çœ‹æ¥é‚£äº›å¤œå‰å·²ç»ç›˜è¸åœ¨çŸ³æ¡¥é™„è¿‘ï¼Œåˆ«äººå¾ˆéš¾é è¿‘ã€‚";
	return action;
end

function Task_00080602_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘ä»¬å¿…é¡»æ‹¿ä¸‹çŸ³æ¡¥ï¼Œä¸è®©é‚£äº›å¦–æ€ªä¸ºè™ä½œä¼¥ã€‚";
	action.m_ActionMsg = "æˆ‘ç°åœ¨å°±å»å‰¿æ€ä»–ä»¬ã€‚";
	return action;
end

function Task_00080602_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è®©é‚£äº›æ€ªç‰©è§é¬¼å»å§ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080602_step_table = {
		[1] = Task_00080602_step_01,
		[2] = Task_00080602_step_02,
		[10] = Task_00080602_step_10,
		};

function Task_00080602_step(step)
	if Task_00080602_step_table[step] ~= nil then
		return Task_00080602_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080602_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080602() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80602) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00080602_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80602) then
		return false;
	end


	player:AddExp(600000);
	player:getCoin(308000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080602_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80602);
end
