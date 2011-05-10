--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090109()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90109) or task:HasCompletedTask(90109) or task:HasSubmitedTask(90109) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90108) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90108) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090109()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90109) or task:HasCompletedTask(90109) or task:HasSubmitedTask(90109) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90108) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90108) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090109()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90109) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090109(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90109) == npcId and Task_Accept_00090109 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90109
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ„Ÿæ¿€ä¹‹æƒ…";
	elseif task:GetTaskSubmitNpc(90109) == npcId then
		if Task_Submit_00090109() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90109
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ„Ÿæ¿€ä¹‹æƒ…";
		elseif task:HasAcceptedTask(90109) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90109
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ„Ÿæ¿€ä¹‹æƒ…";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090109_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "æˆ‘ç»ˆäºå¯ä»¥æ¸…é™çš„ç¡ä¸Šä¸€è§‰äº†ã€‚";
	action.m_ActionMsg = "é‚£å°±å¥½ï¼Œæˆ‘å¯ä»¥é¡ºåˆ©çš„å›å»å¤å‘½äº†ã€‚";
	return action;
end

function Task_00090109_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å—¯ï¼Œå¤šè°¢ä½ ï¼Œå‹‡å£«ã€‚ä¹Ÿè¯·ä½ å¸®æˆ‘å‘å·«å¸ˆè¡¨è¾¾è°¢æ„ã€‚";
	action.m_ActionMsg = "å—¯,ç¥ä½ æœ‰ä¸ªå¥½æ¢¦ã€‚";
	return action;
end

function Task_00090109_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¤šè°¢ä½ å¸®å¥¹ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090109_step_table = {
		[1] = Task_00090109_step_01,
		[2] = Task_00090109_step_02,
		[10] = Task_00090109_step_10,
		};

function Task_00090109_step(step)
	if Task_00090109_step_table[step] ~= nil then
		return Task_00090109_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090109_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090109() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90109) then
		return false;
	end
	task:AddTaskStep(90109);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090109_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90109) then
		return false;
	end


	player:AddExp(575000);
	player:getCoin(306000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090109_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90109);
end
