--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00062207()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(62207) or task:HasCompletedTask(62207) or task:HasSubmitedTask(62207) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00062207()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(62207) or task:HasCompletedTask(62207) or task:HasSubmitedTask(62207) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00062207()
	if GetPlayer():GetTaskMgr():HasCompletedTask(62207) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00062207(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(62207) == npcId and Task_Accept_00062207 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 62207
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æçŒ®æˆ˜åŠŸ";
	elseif task:GetTaskSubmitNpc(62207) == npcId then
		if Task_Submit_00062207() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62207
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æçŒ®æˆ˜åŠŸ";
		elseif task:HasAcceptedTask(62207) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62207
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æçŒ®æˆ˜åŠŸ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00062207_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é½å¿ƒååŠ›æ‰èƒ½ä½¿å®—æ—å˜çš„å¼ºå¤§ï¼Œå˜å¼ºå¤§çš„ç¬¬ä¸€æ­¥å°±æ˜¯æå‡å®—æ—ç­‰çº§ï¼Œä¸ºè‡ªå·±çš„å®—æ—ç­‰çº§å»æçŒ®ä¸€æ¬¡æˆ˜åŠŸå§ï¼";
	action.m_ActionMsg = "è¿™è¿˜ä¸å®¹æ˜“ï¼æˆ‘è¿™å°±å»æçŒ®ä¸€æ¬¡ã€‚";
	return action;
end

function Task_00062207_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ çš„å®—æ—å› ä½ è€Œå¼ºå¤§ï¼";
	action.m_ActionMsg = "";
	return action;
end

local Task_00062207_step_table = {
		[1] = Task_00062207_step_01,
		[10] = Task_00062207_step_10,
		};

function Task_00062207_step(step)
	if Task_00062207_step_table[step] ~= nil then
		return Task_00062207_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00062207_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00062207() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(62207) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00062207_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(62207) then
		return false;
	end


	player:AddExp(20000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00062207_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(62207);
end
