--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080105()
	local player = GetPlayer();
	if player:GetLev() < 65 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80105) or task:HasCompletedTask(80105) or (task:HasSubmitedTask(80105) and (GetSharpDay(task:GetTaskEndTime(80105)) == GetSharpDay(os.time()))) then 
		return false;
	end
	local otherTasks = { 80101, 80102, 80103, 80108, 80109, 80110, 80114, 80115, 80116, 80117, 80118, 80119 };
	for i, v in pairs(otherTasks) do
		if task:HasAcceptedTask(v) or task:HasCompletedTask(v) or (task:HasSubmitedTask(v) and (GetSharpDay(task:GetTaskEndTime(v)) == GetSharpDay(os.time()))) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00080105()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 65 then
		return false;
	end
	if task:HasAcceptedTask(80105) or task:HasCompletedTask(80105) or (task:HasSubmitedTask(80105) and (GetSharpDay(task:GetTaskEndTime(80105)) == GetSharpDay(os.time()))) then 
		return false;
	end
	local otherTasks = { 80101, 80102, 80103, 80108, 80109, 80110, 80114, 80115, 80116, 80117, 80118, 80119 };
	for i, v in pairs(otherTasks) do
		if task:HasAcceptedTask(v) or task:HasCompletedTask(v) or (task:HasSubmitedTask(v) and (GetSharpDay(task:GetTaskEndTime(v)) == GetSharpDay(os.time()))) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00080105()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80105) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080105(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80105) == npcId and Task_Accept_00080105 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80105
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¤©äººé­”ç‹ä¹‹å‹˜";
	elseif task:GetTaskSubmitNpc(80105) == npcId then
		if Task_Submit_00080105() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80105
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¤©äººé­”ç‹ä¹‹å‹˜";
		elseif task:HasAcceptedTask(80105) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80105
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¤©äººé­”ç‹ä¹‹å‹˜";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080105_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "è®¸ä¹…ä¸è§,ä½ å·²æˆé•¿ä¸å°‘.";
	action.m_ActionMsg = "æ‰˜æ‚¨çš„ç¦,æˆ‘åœ¨é‚£å¤©äººåƒäº†ä¸å°‘è‹¦å¤´,ç¡®ä¹Ÿæˆé•¿å¾ˆå¿«.";
	return action;
end

function Task_00080105_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é‚£å°±å¥½,åƒå¾—è‹¦ä¸­è‹¦æ–¹ä¸ºäººä¸Šäººå˜›,æ­‡æ¯ç‰‡åˆ»å°±å‡ºå‘å§.";
	action.m_ActionMsg = "æˆ‘ä¸€å®šä¸è´Ÿä¼—æœ›.";
	return action;
end

function Task_00080105_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æœç„¶é•¿è¿›ä¸å°‘.";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080105_step_table = {
		[1] = Task_00080105_step_01,
		[2] = Task_00080105_step_02,
		[10] = Task_00080105_step_10,
		};

function Task_00080105_step(step)
	if Task_00080105_step_table[step] ~= nil then
		return Task_00080105_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080105_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080105() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80105) then
		return false;
	end
	local otherTasks = { 80101, 80102, 80103, 80108, 80109, 80110, 80114, 80115, 80116, 80117, 80118, 80119 };
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
function Task_00080105_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80105) then
		return false;
	end


	player:AddExp(100000);
	player:getCoin(60000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080105_abandon()
	local task = GetPlayer():GetTaskMgr();
	if task:AbandonTask(80105) then
		if not task:TaskExist(80104) and not task:TaskExist(80106) then
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
