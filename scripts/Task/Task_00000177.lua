--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000177()
	local player = GetPlayer();
	if player:GetLev() < 85 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(177) or task:HasCompletedTask(177) or task:HasSubmitedTask(177) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000177()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 85 then
		return false;
	end
	if task:HasAcceptedTask(177) or task:HasCompletedTask(177) or task:HasSubmitedTask(177) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000177()
	if GetPlayer():GetTaskMgr():HasCompletedTask(177) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000177(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(177) == npcId and Task_Accept_00000177 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 177
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "åŠè¾¹å¤§å¸ˆ";
	elseif task:GetTaskSubmitNpc(177) == npcId then
		if Task_Submit_00000177() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 177
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "åŠè¾¹å¤§å¸ˆ";
		elseif task:HasAcceptedTask(177) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 177
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "åŠè¾¹å¤§å¸ˆ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000177_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ ä¿®è¡Œé€Ÿåº¦çœŸå¿«ï¼Œæœç„¶å¤©èµ‹å¼‚ç¦€ï¼Œæ–¹æ‰æ­¦å½“åŠè¾¹å¤§å¸ˆé£å‰‘ä¼ ä¹¦æ‰¾ä½ ï¼Œè®©å»æ­¦å½“ä¸€è¡Œï¼Œä¼¼ä¹æœ‰ä»€ä¹ˆäº‹æ‰¾ä½ ã€‚";
	action.m_ActionMsg = "æ˜¯å—ï¼Œæˆ‘è¿™å°±å»çœ‹çœ‹ã€‚";
	return action;
end

function Task_00000177_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ æˆ‘ä»¬åˆè§é¢äº†";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000177_step_table = {
		[1] = Task_00000177_step_01,
		[10] = Task_00000177_step_10,
		};

function Task_00000177_step(step)
	if Task_00000177_step_table[step] ~= nil then
		return Task_00000177_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000177_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000177() then
		return false;
	end
	if not task:AcceptTask(177) then
		return false;
	end
	task:AddTaskStep(177);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000177_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(177) then
		return false;
	end


	player:AddExp(150000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000177_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(177);
end
