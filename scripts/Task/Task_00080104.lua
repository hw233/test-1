--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080104()
	local player = GetPlayer();
	if player:GetLev() < 60 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80104) or task:HasCompletedTask(80104) or (task:HasSubmitedTask(80104) and (GetSharpDay(task:GetTaskEndTime(80104)) == GetSharpDay(os.time()))) then 
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
function Task_Can_Accept_00080104()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 60 then
		return false;
	end
	if task:HasAcceptedTask(80104) or task:HasCompletedTask(80104) or (task:HasSubmitedTask(80104) and (GetSharpDay(task:GetTaskEndTime(80104)) == GetSharpDay(os.time()))) then 
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
function Task_Submit_00080104()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80104) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080104(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80104) == npcId and Task_Accept_00080104 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80104
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¤©äººé­”ç‹ä¹‹æ…•";
	elseif task:GetTaskSubmitNpc(80104) == npcId then
		if Task_Submit_00080104() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80104
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¤©äººé­”ç‹ä¹‹æ…•";
		elseif task:HasAcceptedTask(80104) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80104
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¤©äººé­”ç‹ä¹‹æ…•";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080104_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "ä½ æƒ³å»å¤©äººç¦é‚¸ï¼Ÿé‚£é‡Œæœ‰è®¸å¤šé­”æ€ªï¼Œéš¾é“ä½ ä¸æ€•å—ï¼Ÿ";
	action.m_ActionMsg = "æˆ‘æ¥çš„ç›®çš„å°±æ˜¯è¦å»æ€äº†ä»–ä»¬ã€‚";
	return action;
end

function Task_00080104_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é‚£ä½ å°±å…ˆå»æ€äº†ççµå…½-æ…•ï¼Œä»–å°†æ˜¯ä½ åœ¨å¤©äººç¦é‚¸é‡åˆ°çš„ç¬¬ä¸€ä¸ªé­”ç‹ï¼Œè®©æˆ‘çœ‹çœ‹ä½ çš„å®åŠ›å¦‚ä½•ã€‚";
	action.m_ActionMsg = "ççµå…½-æ…•ï¼Ÿè¿™å°±æ˜¯é‚£æ€ªç‰©çš„åå­—ï¼Ÿå¾…æˆ‘å»ä¼šä¸€ä¼šä»–ã€‚";
	return action;
end

function Task_00080104_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™æ¬¡ä¿®ç‚¼æ„Ÿè§‰å¦‚ä½•?";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080104_step_table = {
		[1] = Task_00080104_step_01,
		[2] = Task_00080104_step_02,
		[10] = Task_00080104_step_10,
		};

function Task_00080104_step(step)
	if Task_00080104_step_table[step] ~= nil then
		return Task_00080104_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080104_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080104() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80104) then
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
function Task_00080104_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80104) then
		return false;
	end


	player:AddExp(80000);
	player:getCoin(50000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080104_abandon()
	local task = GetPlayer():GetTaskMgr();
	if task:AbandonTask(80104) then
		if not task:TaskExist(80105) and not task:TaskExist(80106) then
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
