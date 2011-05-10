--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00010301()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 18 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10301) or task:HasCompletedTask(10301) or task:HasSubmitedTask(10301) then
		return false;
	end
	if not task:HasSubmitedTask(10204) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00010301()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 18 then
		return false;
	end
	if task:HasAcceptedTask(10301) or task:HasCompletedTask(10301) or task:HasSubmitedTask(10301) then
		return false;
	end
	if not task:HasSubmitedTask(10204) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00010301()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10301) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00010301(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10301) == npcId and Task_Accept_00010301 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10301
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "åˆåˆ°äº‘å²©å¯º";
	elseif task:GetTaskSubmitNpc(10301) == npcId then
		if Task_Submit_00010301() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10301
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "åˆåˆ°äº‘å²©å¯º";
		elseif task:HasAcceptedTask(10301) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10301
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "åˆåˆ°äº‘å²©å¯º";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00010301_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ åˆåˆ°äº‘å²©å¯ºï¼Œè¿˜ä¸ç†Ÿæ‚‰è¿™é‡Œçš„æƒ…å†µï¼Œä¸è¿‡ä¸€ç›´æœ‰æ¶ˆæ¯è¯´æ«æ¡¥é•‡é™„è¿‘æœ‰äººåœ¨åˆ°å¤„æ‰“å¬æ‚¦æ„ç‹å¦ƒçš„ä¸‹è½ï¼Œä½ æ­£å¥½å‡ºå»è½¬è½¬ï¼Œé¡ºä¾¿æ‰“å¬ä¸‹æ˜¯æ€ä¹ˆå›äº‹ã€‚";
	action.m_ActionMsg = "è¿™å€’æ˜¯å¾ˆå¥‡æ€ªï¼Œæˆ‘å‡ºåŸå»æŸ¥çœ‹ä¸‹æƒ…å†µã€‚";
	return action;
end

function Task_00010301_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç»ˆäºç­‰åˆ°äº†ä¸€ä¸ªå¤©æ—çš„å‹‡å£«ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010301_step_table = {
		[1] = Task_00010301_step_01,
		[10] = Task_00010301_step_10,
		};

function Task_00010301_step(step)
	if Task_00010301_step_table[step] ~= nil then
		return Task_00010301_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00010301_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010301() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(10301) then
		return false;
	end
	task:AddTaskStep(10301);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00010301_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(10301) then
		return false;
	end


	player:AddExp(1800);
	player:getCoin(1500);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00010301_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10301);
end
