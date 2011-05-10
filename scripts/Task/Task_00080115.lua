--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080115()
	local player = GetPlayer();
	if player:GetLev() < 80 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80115) or task:HasCompletedTask(80115) or (task:HasSubmitedTask(80115) and (GetSharpDay(task:GetTaskEndTime(80115)) == GetSharpDay(os.time()))) then 
		return false;
	end
	local otherTasks = { 80101, 80102, 80103, 80104, 80105, 80106, 80108, 80109, 80110, 80117, 80118, 80119 };
	for i, v in pairs(otherTasks) do
		if task:HasAcceptedTask(v) or task:HasCompletedTask(v) or (task:HasSubmitedTask(v) and (GetSharpDay(task:GetTaskEndTime(v)) == GetSharpDay(os.time()))) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00080115()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 80 then
		return false;
	end
	if task:HasAcceptedTask(80115) or task:HasCompletedTask(80115) or (task:HasSubmitedTask(80115) and (GetSharpDay(task:GetTaskEndTime(80115)) == GetSharpDay(os.time()))) then 
		return false;
	end
	local otherTasks = { 80101, 80102, 80103, 80104, 80105, 80106, 80108, 80109, 80110, 80117, 80118, 80119 };
	for i, v in pairs(otherTasks) do
		if task:HasAcceptedTask(v) or task:HasCompletedTask(v) or (task:HasSubmitedTask(v) and (GetSharpDay(task:GetTaskEndTime(v)) == GetSharpDay(os.time()))) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00080115()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80115) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080115(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80115) == npcId and Task_Accept_00080115 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80115
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ— é—´ç‚¼ç‹±ä¹‹ç¥è²‚ä¾ å•";
	elseif task:GetTaskSubmitNpc(80115) == npcId then
		if Task_Submit_00080115() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80115
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ— é—´ç‚¼ç‹±ä¹‹ç¥è²‚ä¾ å•";
		elseif task:HasAcceptedTask(80115) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80115
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ— é—´ç‚¼ç‹±ä¹‹ç¥è²‚ä¾ å•";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080115_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä¸€ä»£é­”ç‹Â·å•å¸ƒå’Œé­”åÂ·å¤±å¿ƒçš„è²‚è‰ä½äºæ— é—´ç‚¼ç‹±çš„ç¬¬15å±‚ï¼Œä»–ä»¬è¢«åäººç§°ä¸ºç¥è²‚ä¾ å•ï¼Œæƒ³è¦å‡»è´¥ä»–ä»¬ä¸å®¹æ˜“ï¼Œè¯·å‹‡å£«é‡åŠ›è€Œä¸ºã€‚";
	action.m_ActionMsg = "æˆ‘ä¼šçš„ï¼Œè¯·å§‘å¨˜æ”¾å¿ƒ";
	return action;
end

function Task_00080115_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘å·²å°†é­”ç‹Â·å•å¸ƒå’Œé­”åÂ·å¤±å¿ƒçš„è²‚è‰å‡»è´¥";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080115_step_table = {
		[1] = Task_00080115_step_01,
		[10] = Task_00080115_step_10,
		};

function Task_00080115_step(step)
	if Task_00080115_step_table[step] ~= nil then
		return Task_00080115_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080115_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080115() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80115) then
		return false;
	end
	local otherTasks = { 80101, 80102, 80103, 80104, 80105, 80106, 80108, 80109, 80110, 80117, 80118, 80119 };
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
function Task_00080115_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80115) then
		return false;
	end


	player:AddExp(200000);
	player:getCoin(200000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080115_abandon()
	local task = GetPlayer():GetTaskMgr();
	if task:AbandonTask(80115) then
		if not task:TaskExist(80114) and not task:TaskExist(80116) then
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
