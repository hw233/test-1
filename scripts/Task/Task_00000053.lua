--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000053()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 35 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(53) or task:HasCompletedTask(53) or task:HasSubmitedTask(53) then
		return false;
	end
	if not task:HasSubmitedTask(51) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000053()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 35 then
		return false;
	end
	if task:HasAcceptedTask(53) or task:HasCompletedTask(53) or task:HasSubmitedTask(53) then
		return false;
	end
	if not task:HasSubmitedTask(51) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000053()
	if GetPlayer():GetTaskMgr():HasCompletedTask(53) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000053(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(53) == npcId and Task_Accept_00000053 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 53
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ¡‚èŠ±å±±";
	elseif task:GetTaskSubmitNpc(53) == npcId then
		if Task_Submit_00000053() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 53
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ¡‚èŠ±å±±";
		elseif task:HasAcceptedTask(53) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 53
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ¡‚èŠ±å±±";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000053_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é­å®¶åœºä¸€æˆ˜ï¼Œé¡½çŸ³å¤§å¸ˆä¸å¹¸è¢«é‚£ä¸ƒæ‰‹å¤œå‰é¾™é£çš„å­æ¯é˜´é­‚å‰‘æ‰€ä¼¤ï¼Œå¿…é¡»è¦æ¡‚èŠ±å±±é‡‘èŠ±å§¥å§¥é‚£é‡Œçš„ä¹Œé£è‰æ‰èƒ½æ•‘æ²»ï¼Œä½ èµ¶å¿«å»æ¡‚èŠ±å±±æ‰¾é‡‘èŠ±å§¥å§¥çš„å¾’å¼Ÿç”³è‹¥å…°å•†é‡ä¸€ä¸‹å¦‚ä½•é‡‡é›†ä¹Œé£è‰ã€‚";
	action.m_ActionMsg = "å¼Ÿå­è¿™å°±å»æ¡‚èŠ±å±±ã€‚";
	return action;
end

function Task_00000053_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é¡½çŸ³å¤§å¸ˆè¢«å¦–äººæ‰€ä¼¤ï¼ŒçœŸæ˜¯ä¸å¹¸ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000053_step_table = {
		[1] = Task_00000053_step_01,
		[10] = Task_00000053_step_10,
		};

function Task_00000053_step(step)
	if Task_00000053_step_table[step] ~= nil then
		return Task_00000053_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000053_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000053() then
		return false;
	end
	if not task:AcceptTask(53) then
		return false;
	end
	task:AddTaskStep(53);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000053_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(53) then
		return false;
	end


	player:AddExp(5000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000053_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(53);
end
