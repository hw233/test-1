--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000095()
	local player = GetPlayer();
	if player:GetLev() < 50 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(95) or task:HasCompletedTask(95) or task:HasSubmitedTask(95) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000095()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 50 then
		return false;
	end
	if task:HasAcceptedTask(95) or task:HasCompletedTask(95) or task:HasSubmitedTask(95) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000095()
	if GetPlayer():GetTaskMgr():HasCompletedTask(95) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000095(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(95) == npcId and Task_Accept_00000095 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 95
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç™½äº‘å¤§å¸ˆ";
	elseif task:GetTaskSubmitNpc(95) == npcId then
		if Task_Submit_00000095() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 95
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç™½äº‘å¤§å¸ˆ";
		elseif task:HasAcceptedTask(95) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 95
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç™½äº‘å¤§å¸ˆ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000095_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ è¿˜è®°å¾—åœ¨æˆéƒ½éƒŠå¤–ç™½äº‘å¯ºé‡è§çš„ç™½äº‘å¤§å¸ˆå—ï¼Ÿå¥¹ä¸€èˆ¬éƒ½åœ¨å§‘å©†å²­é™„è¿‘çš„æ­£ä¿®åºµéšå±…ï¼Œæˆ‘ä¸å¥¹æœ‰å¤šå¹´æœªè§ï¼Œä¸çŸ¥é“ä½ å¯å¦ä»£æˆ‘å»é—®å€™å¥¹ä¸€ä¸‹ï¼Ÿ";
	action.m_ActionMsg = "å¥½çš„ï¼Œæˆ‘è¿™å°±å»å§‘å©†å²­ã€‚";
	return action;
end

function Task_00000095_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."æˆéƒ½éƒŠå¤–ä¸ä½ ä¸€åˆ«ï¼Œæƒ³ä¸åˆ°æˆ‘ä»¬åˆè§é¢äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000095_step_table = {
		[1] = Task_00000095_step_01,
		[10] = Task_00000095_step_10,
		};

function Task_00000095_step(step)
	if Task_00000095_step_table[step] ~= nil then
		return Task_00000095_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000095_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000095() then
		return false;
	end
	if not task:AcceptTask(95) then
		return false;
	end
	task:AddTaskStep(95);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000095_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(95) then
		return false;
	end


	player:AddExp(30000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000095_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(95);
end
