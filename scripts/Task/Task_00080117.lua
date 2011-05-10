--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080117()
	local player = GetPlayer();
	if player:GetLev() < 90 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80117) or task:HasCompletedTask(80117) or (task:HasSubmitedTask(80117) and (GetSharpDay(task:GetTaskEndTime(80117)) == GetSharpDay(os.time()))) then 
		return false;
	end
	local otherTasks = { 80101, 80102, 80103, 80104, 80105, 80106, 80108, 80109, 80110, 80114, 80115, 80116 };
	for i, v in pairs(otherTasks) do
		if task:HasAcceptedTask(v) or task:HasCompletedTask(v) or (task:HasSubmitedTask(v) and (GetSharpDay(task:GetTaskEndTime(v)) == GetSharpDay(os.time()))) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00080117()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 90 then
		return false;
	end
	if task:HasAcceptedTask(80117) or task:HasCompletedTask(80117) or (task:HasSubmitedTask(80117) and (GetSharpDay(task:GetTaskEndTime(80117)) == GetSharpDay(os.time()))) then 
		return false;
	end
	local otherTasks = { 80101, 80102, 80103, 80104, 80105, 80106, 80108, 80109, 80110, 80114, 80115, 80116 };
	for i, v in pairs(otherTasks) do
		if task:HasAcceptedTask(v) or task:HasCompletedTask(v) or (task:HasSubmitedTask(v) and (GetSharpDay(task:GetTaskEndTime(v)) == GetSharpDay(os.time()))) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00080117()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80117) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080117(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80117) == npcId and Task_Accept_00080117 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80117
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æµ·åº•å¹»å¢ƒä¹‹å‘¨ç‘œä¹‹å½±";
	elseif task:GetTaskSubmitNpc(80117) == npcId then
		if Task_Submit_00080117() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80117
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æµ·åº•å¹»å¢ƒä¹‹å‘¨ç‘œä¹‹å½±";
		elseif task:HasAcceptedTask(80117) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80117
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æµ·åº•å¹»å¢ƒä¹‹å‘¨ç‘œä¹‹å½±";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080117_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ²™æ³¢æµ·åº•å¤çªŸä¸­æ½œè—ç€è®¸å¤šä¸ä¸ºäººçŸ¥çš„ç§˜å¯†ï¼Œå¦‚æœå‡»è´¥å‘¨ç‘œä¹‹å½±ä¸çŸ¥ä¼šå¾—åˆ°ä»€ä¹ˆç¨€ä¸–çå®ï¼Œä½ æœ‰å…´è¶£ä¸€è¯•å—ï¼Ÿ";
	action.m_ActionMsg = "æˆ‘è·ƒè·ƒæ¬²è¯•";
	return action;
end

function Task_00080117_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘å·²å°†å‘¨ç‘œä¹‹å½±å‡»è´¥";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080117_step_table = {
		[1] = Task_00080117_step_01,
		[10] = Task_00080117_step_10,
		};

function Task_00080117_step(step)
	if Task_00080117_step_table[step] ~= nil then
		return Task_00080117_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080117_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080117() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80117) then
		return false;
	end
	local otherTasks = { 80101, 80102, 80103, 80104, 80105, 80106, 80108, 80109, 80110, 80114, 80115, 80116 };
	for i, v in pairs(otherTasks) do
		if i ~= #otherTasks then
			task:DelCanAcceptTask2(v, false);
		else
			task:DelCanAcceptTask2(v, true);
		end
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00080117_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80117) then
		return false;
	end


	player:AddExp(300000);
	player:getCoin(250000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080117_abandon()
	local task = GetPlayer():GetTaskMgr();
	if task:AbandonTask(80117) then
		if not task:TaskExist(80118) and not task:TaskExist(80119) then
			local otherTasks = { 80101, 80102, 80103, 80104, 80105, 80106, 80108, 80109, 80110, 80114, 80115, 80116, 80117, 80118, 80119};
			for i, v in pairs(otherTasks) do
				if task:TaskCanAccept(v) then
					task:AddCanAcceptTask2(v, false);
				end
			end
			task:SendCanAcceptTaskInfor();
		end
		return true;
	end
	return false;
end
