--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080106()
	local player = GetPlayer();
	if player:GetLev() < 70 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80106) or task:HasCompletedTask(80106) or (task:HasSubmitedTask(80106) and (GetSharpDay(task:GetTaskEndTime(80106)) == GetSharpDay(os.time()))) then 
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
function Task_Can_Accept_00080106()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 70 then
		return false;
	end
	if task:HasAcceptedTask(80106) or task:HasCompletedTask(80106) or (task:HasSubmitedTask(80106) and (GetSharpDay(task:GetTaskEndTime(80106)) == GetSharpDay(os.time()))) then 
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
function Task_Submit_00080106()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80106) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080106(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80106) == npcId and Task_Accept_00080106 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80106
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¤©äººé­”ç‹ä¹‹ç½—ä¸¥";
	elseif task:GetTaskSubmitNpc(80106) == npcId then
		if Task_Submit_00080106() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80106
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¤©äººé­”ç‹ä¹‹ç½—ä¸¥";
		elseif task:HasAcceptedTask(80106) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80106
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¤©äººé­”ç‹ä¹‹ç½—ä¸¥";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080106_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "åšæŒåˆ°åº•,èƒœåˆ©ä¸è¿œäº†.";
	action.m_ActionMsg = "é‚£ä¹ˆå¤šè‹¦éš¾éƒ½æ‰›è¿‡æ¥äº†,è¿™äº›ä¸ç®—ä»€ä¹ˆ.";
	return action;
end

function Task_00080106_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¾ˆå¥½,å›å»å¤©äººç¦é‚¸å®Œæˆä½ åœ¨é‚£é‡Œæœ€åçš„ä¿®ç‚¼å§.";
	action.m_ActionMsg = "å¥½çš„,æˆ‘è¿™å°±å».";
	return action;
end

function Task_00080106_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ éœ€è¦å»æ›´å¤æ‚çš„åœ°æ–¹ä¿®ç‚¼äº†.";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080106_step_table = {
		[1] = Task_00080106_step_01,
		[2] = Task_00080106_step_02,
		[10] = Task_00080106_step_10,
		};

function Task_00080106_step(step)
	if Task_00080106_step_table[step] ~= nil then
		return Task_00080106_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080106_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080106() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80106) then
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
function Task_00080106_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80106) then
		return false;
	end


	player:AddExp(120000);
	player:getCoin(70000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080106_abandon()
	local task = GetPlayer():GetTaskMgr();
	if task:AbandonTask(80106) then
		if not task:TaskExist(80104) and not task:TaskExist(80105) then
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
