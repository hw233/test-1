--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000158()
	local player = GetPlayer();
	if player:GetLev() < 75 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(158) or task:HasCompletedTask(158) or task:HasSubmitedTask(158) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000158()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 75 then
		return false;
	end
	if task:HasAcceptedTask(158) or task:HasCompletedTask(158) or task:HasSubmitedTask(158) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000158()
	if GetPlayer():GetTaskMgr():HasCompletedTask(158) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000158(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(158) == npcId and Task_Accept_00000158 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 158
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¤©ç‹æ¸¡åŠ«";
	elseif task:GetTaskSubmitNpc(158) == npcId then
		if Task_Submit_00000158() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 158
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¤©ç‹æ¸¡åŠ«";
		elseif task:HasAcceptedTask(158) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 158
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¤©ç‹æ¸¡åŠ«";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000158_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¦‚ä»Šåœ¨ä¸œæµ·æœ‰ä¸€ä½é“å‹æ­£è¦æ¸¡åŠ«ï¼Œå¥¹åŸæœ¬ä¹ƒæ˜¯ä¸€å¤©ç‹ï¼Œæ˜¯ç§¦æ°å§å¦¹çš„æ¯äº²ï¼Œå·²ç»æœ‰æ•°ä½æˆ‘ä»¬æ­£é“ä¸­äººå‰å»åŠ©å¥¹æ¸¡åŠ«ï¼Œå°å‹ä½ ä¸å¦‚ä¹Ÿå»ä¸œæµ·è§è¯†ä¸€ä¸‹ã€‚";
	action.m_ActionMsg = "å—¯ï¼Œæˆ‘è¿™å°±å»ã€‚";
	return action;
end

function Task_00000158_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¤šè°¢è¯¸ä½é“å‹ç›¸åŠ©ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000158_step_table = {
		[1] = Task_00000158_step_01,
		[10] = Task_00000158_step_10,
		};

function Task_00000158_step(step)
	if Task_00000158_step_table[step] ~= nil then
		return Task_00000158_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000158_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000158() then
		return false;
	end
	if not task:AcceptTask(158) then
		return false;
	end
	task:AddTaskStep(158);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000158_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(158) then
		return false;
	end


	player:AddExp(80000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000158_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(158);
end
