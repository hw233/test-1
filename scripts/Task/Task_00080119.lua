--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080119()
	local player = GetPlayer();
	if player:GetLev() < 90 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80119) or task:HasCompletedTask(80119) or (task:HasSubmitedTask(80119) and (GetSharpDay(task:GetTaskEndTime(80119)) == GetSharpDay(os.time()))) then 
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
function Task_Can_Accept_00080119()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 90 then
		return false;
	end
	if task:HasAcceptedTask(80119) or task:HasCompletedTask(80119) or (task:HasSubmitedTask(80119) and (GetSharpDay(task:GetTaskEndTime(80119)) == GetSharpDay(os.time()))) then 
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
function Task_Submit_00080119()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80119) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080119(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80119) == npcId and Task_Accept_00080119 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80119
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æµ·åº•å¹»å¢ƒä¹‹æ¡ƒå›­ç»“ä¹‰";
	elseif task:GetTaskSubmitNpc(80119) == npcId then
		if Task_Submit_00080119() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80119
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æµ·åº•å¹»å¢ƒä¹‹æ¡ƒå›­ç»“ä¹‰";
		elseif task:HasAcceptedTask(80119) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80119
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æµ·åº•å¹»å¢ƒä¹‹æ¡ƒå›­ç»“ä¹‰";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080119_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¬éš”å£è€æ¿å¨˜è¯´ä½äº20å±‚çš„æµ·åº•å¹»å¢ƒæœ‰åˆ˜å…³å¼ ä¸‰äººä¹‹å½±ï¼Œæ›¾ç»æœ‰ä¸ªä½å®¿è¿‡å¥¹æ—…åº—çš„ç¥ç§˜å¯»å®äººå·²ç»ä»é‚£è·å¾—äº†å¥³å¨²çš„ç¨€ä¸–çå®ï¼Œä¸çŸ¥ä½ æ˜¯å¦æœ‰å…´è¶£";
	action.m_ActionMsg = "æˆ‘ç«‹åˆ»ä¸Šè·¯";
	return action;
end

function Task_00080119_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘ç«‹åˆ»ä¸Šè·¯";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080119_step_table = {
		[1] = Task_00080119_step_01,
		[10] = Task_00080119_step_10,
		};

function Task_00080119_step(step)
	if Task_00080119_step_table[step] ~= nil then
		return Task_00080119_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080119_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080119() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80119) then
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
function Task_00080119_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80119) then
		return false;
	end


	player:AddExp(300000);
	player:getCoin(250000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080119_abandon()
	local task = GetPlayer():GetTaskMgr();
	if task:AbandonTask(80119) then
		if not task:TaskExist(80117) and not task:TaskExist(80118) then
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
