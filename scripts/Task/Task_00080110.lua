--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080110()
	local player = GetPlayer();
	if player:GetLev() < 80 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80110) or task:HasCompletedTask(80110) or (task:HasSubmitedTask(80110) and (GetSharpDay(task:GetTaskEndTime(80110)) == GetSharpDay(os.time()))) then 
		return false;
	end
	local otherTasks = { 80101, 80102, 80103, 80104, 80105, 80106, 80114, 80115, 80116, 80117, 80118, 80119 };
	for i, v in pairs(otherTasks) do
		if task:HasAcceptedTask(v) or task:HasCompletedTask(v) or (task:HasSubmitedTask(v) and (GetSharpDay(task:GetTaskEndTime(v)) == GetSharpDay(os.time()))) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00080110()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 80 then
		return false;
	end
	if task:HasAcceptedTask(80110) or task:HasCompletedTask(80110) or (task:HasSubmitedTask(80110) and (GetSharpDay(task:GetTaskEndTime(80110)) == GetSharpDay(os.time()))) then 
		return false;
	end
	local otherTasks = { 80101, 80102, 80103, 80104, 80105, 80106, 80114, 80115, 80116, 80117, 80118, 80119 };
	for i, v in pairs(otherTasks) do
		if task:HasAcceptedTask(v) or task:HasCompletedTask(v) or (task:HasSubmitedTask(v) and (GetSharpDay(task:GetTaskEndTime(v)) == GetSharpDay(os.time()))) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00080110()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80110) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080110(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80110) == npcId and Task_Accept_00080110 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80110
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å›°ä»™é­”ç‹ä¹‹ç§¦ä»™äºº";
	elseif task:GetTaskSubmitNpc(80110) == npcId then
		if Task_Submit_00080110() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80110
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å›°ä»™é­”ç‹ä¹‹ç§¦ä»™äºº";
		elseif task:HasAcceptedTask(80110) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80110
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å›°ä»™é­”ç‹ä¹‹ç§¦ä»™äºº";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080110_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç§¦ä»™äººæ˜¯å›°ä»™é˜µæœ€åä¸€ä¸ªå¤§é­”ç‹ï¼Œä»–æ¯”ä»¥å¾€ä½ é‡åˆ°è¿‡çš„é­”æ€ªéƒ½è¦å‰å®³ç™¾å€ï¼Œè¦åŠ å€å°å¿ƒã€‚";
	action.m_ActionMsg = "ä¸ç®¡ä»–æœ‰å¤šå‰å®³æˆ‘éƒ½ä¼šè®©ä»–æ­»äºæˆ‘çš„åˆ€ä¸‹ã€‚";
	return action;
end

function Task_00080110_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ çš„ä¿®ç‚¼å·²ç»åˆ°äº†è¾ƒé«˜å¢ƒç•Œï¼Œä½†åˆ‡è®°éª„å‚²ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080110_step_table = {
		[1] = Task_00080110_step_01,
		[10] = Task_00080110_step_10,
		};

function Task_00080110_step(step)
	if Task_00080110_step_table[step] ~= nil then
		return Task_00080110_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080110_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080110() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80110) then
		return false;
	end
	local otherTasks = { 80101, 80102, 80103, 80104, 80105, 80106, 80114, 80115, 80116, 80117, 80118, 80119 };
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
function Task_00080110_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80110) then
		return false;
	end


	player:AddExp(200000);
	player:getCoin(180000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080110_abandon()
	local task = GetPlayer():GetTaskMgr();
	if task:AbandonTask(80110) then
		if not task:TaskExist(80108) and not task:TaskExist(80109) then
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
