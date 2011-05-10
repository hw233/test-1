--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00050101()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 8 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50101) or task:HasCompletedTask(50101) or task:HasSubmitedTask(50101) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00050101()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 8 then
		return false;
	end
	if task:HasAcceptedTask(50101) or task:HasCompletedTask(50101) or task:HasSubmitedTask(50101) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00050101()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50101) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00050101(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50101) == npcId and Task_Accept_00050101 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50101
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç‹åçš„æ€å¿µ";
	elseif task:GetTaskSubmitNpc(50101) == npcId then
		if Task_Submit_00050101() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50101
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç‹åçš„æ€å¿µ";
		elseif task:HasAcceptedTask(50101) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50101
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç‹åçš„æ€å¿µ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00050101_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "å‹‡å£«ï¼Œä½ ç°åœ¨èƒ½å¦å¸®æˆ‘ä¸ªå¿™å»æ‰¾ä¸€ä¸‹é˜¿ä¿®ç½—ï¼Ÿ";
	action.m_ActionMsg = "ç‹åçš„å©å’ï¼Œå®šç„¶æ²¡æœ‰é—®é¢˜ã€‚";
	return action;
end

function Task_00050101_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "èƒ½éº»çƒ¦ä½ å¸®æˆ‘å»æ‰¾ä¸€ä¸‹é˜¿ä¿®ç½—ï¼Œæˆ‘æœ€è¿‘éå¸¸æƒ³å¿µæˆ‘ä»¬çš„å„¿å­æ–‡éš£ï¼Œé—®ä»–æ„¿ä¸æ„¿æ„é™ªæˆ‘å»ä¸€ä¸‹æ–‡éš£æ›¾ç»ä¿®è¡Œå­¦é“çš„é¾™ç‹åº™ã€‚";
	action.m_ActionMsg = "ç‹åï¼Œè¿™æ ·çš„äº‹æƒ…å…¶å®ä½ ä¹Ÿå¯ä»¥è‡ªå·±å»é—®çš„å˜›ã€‚";
	return action;
end

function Task_00050101_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä¸ºä»€ä¹ˆæ˜¯ç‹åè®©ä½ æ¥çš„ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050101_step_table = {
		[1] = Task_00050101_step_01,
		[2] = Task_00050101_step_02,
		[10] = Task_00050101_step_10,
		};

function Task_00050101_step(step)
	if Task_00050101_step_table[step] ~= nil then
		return Task_00050101_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00050101_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050101() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50101) then
		return false;
	end
	task:AddTaskStep(50101);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00050101_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(50101) then
		return false;
	end


	player:AddExp(850);
	player:getCoin(500);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00050101_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50101);
end
