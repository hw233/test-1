--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000103()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(103) or task:HasCompletedTask(103) or task:HasSubmitedTask(103) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(102) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(102) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(102) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000103()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(103) or task:HasCompletedTask(103) or task:HasSubmitedTask(103) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(102) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(102) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(102) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000103()
	if GetPlayer():GetTaskMgr():HasCompletedTask(103) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000103(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(103) == npcId and Task_Accept_00000103 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 103
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "é’ç‰›å‰‘";
	elseif task:GetTaskSubmitNpc(103) == npcId then
		if Task_Submit_00000103() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 103
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "é’ç‰›å‰‘";
		elseif task:HasAcceptedTask(103) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 103
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "é’ç‰›å‰‘";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000103_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™é’ç‰›å‰‘æˆ‘ä¸€å…±ç‚¼åˆ¶äº†ä¸ƒæŠŠï¼Œå°šæœªè§è¡€ï¼Œ"..GetPlayerName(GetPlayer()).."è¿™æŸ„é’ç‰›å‰‘æˆ‘å°±èµ é€ä¸ä½ ï¼Œæ‹¿å»åŠ©é‚£é‡‘é’ˆåœ£æ¯å…µè§£å§ï¼Œè™½ç„¶å¥¹å½“å¹´ç„å¥³é’ˆæ¯’è¾£å¼‚å¸¸ï¼Œä½†ä¹Ÿç½ªä¸å½“ç»ã€‚";
	action.m_ActionMsg = "å¤šè°¢å‰è¾ˆèµå‰‘ã€‚";
	return action;
end

function Task_00000103_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¤šè°¢å°‘ä¾ ï¼Œæœ‰é’ç‰›å‰‘åŠ©æˆ‘å…µè§£ï¼Œæˆ‘æ€»ç®—å¯ä»¥é¿å¼€æ­¤æ¬¡åŠ«éš¾äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000103_step_table = {
		[1] = Task_00000103_step_01,
		[10] = Task_00000103_step_10,
		};

function Task_00000103_step(step)
	if Task_00000103_step_table[step] ~= nil then
		return Task_00000103_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000103_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000103() then
		return false;
	end
	if not task:AcceptTask(103) then
		return false;
	end
	task:AddTaskStep(103);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000103_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(103) then
		return false;
	end


	player:AddExp(54555);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000103_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(103);
end
