--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090127()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90127) or task:HasCompletedTask(90127) or task:HasSubmitedTask(90127) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90126) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90126) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090127()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90127) or task:HasCompletedTask(90127) or task:HasSubmitedTask(90127) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90126) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90126) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090127()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90127) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090127(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90127) == npcId and Task_Accept_00090127 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90127
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ¯æœ¨è®¡åˆ’";
	elseif task:GetTaskSubmitNpc(90127) == npcId then
		if Task_Submit_00090127() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90127
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ¯æœ¨è®¡åˆ’";
		elseif task:HasAcceptedTask(90127) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90127
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ¯æœ¨è®¡åˆ’";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090127_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "æœç„¶ä¸å‡ºæˆ‘æ‰€æ–™ã€‚";
	action.m_ActionMsg = "é‚£æˆ‘ä»¬ç°åœ¨è¯¥æ€ä¹ˆåŠï¼Ÿ";
	return action;
end

function Task_00090127_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ¯æœ¨è¶å·²æ°”æ€¥è´¥åï¼ŒåŠ¿å¿…ä¼šè¿›æ”»æŒ‡æŒ¥è¥ï¼Œä½ èµ¶ç´§å»é€šçŸ¥å‹’å‹¤ï¼Œè®©ä»–åšå¥½é˜²èŒƒæªæ–½ã€‚";
	action.m_ActionMsg = "å¯æ¶çš„æ¯æœ¨è¶ï¼Œæˆ‘è¿™å°±å»ã€‚";
	return action;
end

function Task_00090127_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "çœ‹æ¥æˆ‘ä»¬è¦å‡†å¤‡å†³ä¸€æ­»æˆ˜äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090127_step_table = {
		[1] = Task_00090127_step_01,
		[2] = Task_00090127_step_02,
		[10] = Task_00090127_step_10,
		};

function Task_00090127_step(step)
	if Task_00090127_step_table[step] ~= nil then
		return Task_00090127_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090127_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090127() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90127) then
		return false;
	end
	task:AddTaskStep(90127);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090127_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90127) then
		return false;
	end


	player:AddExp(725000);
	player:getCoin(318000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090127_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90127);
end
