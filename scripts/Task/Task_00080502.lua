--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080502()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80502) or task:HasCompletedTask(80502) or task:HasSubmitedTask(80502) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80501) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80501) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00080502()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80502) or task:HasCompletedTask(80502) or task:HasSubmitedTask(80502) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80501) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80501) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00080502()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80502) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080502(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80502) == npcId and Task_Accept_00080502 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80502
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ä¸‹é©¬å¨";
	elseif task:GetTaskSubmitNpc(80502) == npcId then
		if Task_Submit_00080502() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80502
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ä¸‹é©¬å¨";
		elseif task:HasAcceptedTask(80502) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80502
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ä¸‹é©¬å¨";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080502_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "ä½ æ˜¯æ¥å¸®æˆ‘çš„å—ï¼Ÿ";
	action.m_ActionMsg = "æ˜¯å¯’å°§è®©æˆ‘æ¥çš„ï¼Œä½ è¿™æœ‰ä»€ä¹ˆéœ€è¦æˆ‘å¸®å¿™çš„å°½ç®¡è¯´ã€‚";
	return action;
end

function Task_00080502_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ æ¥å¾—æ­£å¥½ï¼Œå¸®æˆ‘å»ä¾¦æŸ¥é»‘é£å¯¨ï¼Œå¬è¯´ä»–ä»¬å‹¾ç»“å¤œæ‘©ç›Ÿï¼Œä¸çŸ¥æ˜¯çœŸæ˜¯å‡ï¼Œé¡ºä¾¿æ€å‡ ä¸ªï¼Œç»™ä»–ä»¬ä¸€ä¸ªä¸‹é©¬å¨ã€‚";
	action.m_ActionMsg = "æ‰“å‡»å¤œæ‘©ç›Ÿæˆ‘ä¹‰ä¸å®¹è¾ã€‚";
	return action;
end

function Task_00080502_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é»‘é£å¯¨æƒ…å†µå¦‚ä½•ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080502_step_table = {
		[1] = Task_00080502_step_01,
		[2] = Task_00080502_step_02,
		[10] = Task_00080502_step_10,
		};

function Task_00080502_step(step)
	if Task_00080502_step_table[step] ~= nil then
		return Task_00080502_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080502_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080502() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80502) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00080502_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80502) then
		return false;
	end


	player:AddExp(525000);
	player:getCoin(302000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080502_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80502);
end
