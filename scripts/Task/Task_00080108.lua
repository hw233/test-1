--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080108()
	local player = GetPlayer();
	if player:GetLev() < 75 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80108) or task:HasCompletedTask(80108) or (task:HasSubmitedTask(80108) and (GetSharpDay(task:GetTaskEndTime(80108)) == GetSharpDay(os.time()))) then 
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
function Task_Can_Accept_00080108()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 75 then
		return false;
	end
	if task:HasAcceptedTask(80108) or task:HasCompletedTask(80108) or (task:HasSubmitedTask(80108) and (GetSharpDay(task:GetTaskEndTime(80108)) == GetSharpDay(os.time()))) then 
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
function Task_Submit_00080108()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80108) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080108(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80108) == npcId and Task_Accept_00080108 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80108
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å›°ä»™é­”ç‹ä¹‹ä¸‰é­”é—¹ä¸–";
	elseif task:GetTaskSubmitNpc(80108) == npcId then
		if Task_Submit_00080108() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80108
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å›°ä»™é­”ç‹ä¹‹ä¸‰é­”é—¹ä¸–";
		elseif task:HasAcceptedTask(80108) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80108
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å›°ä»™é­”ç‹ä¹‹ä¸‰é­”é—¹ä¸–";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080108_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "å›°ä»™è¿™ä¸‰ä¸ªé­”å¤´å¯ä¸æ˜¯å¥½å¯¹ä»˜çš„ï¼Œä»–ä»¬æ€äººæ— æ•°ï¼Œä¸å°‘å‹‡å£«ç‰ºç‰²åœ¨é‚£ã€‚";
	action.m_ActionMsg = "æˆ‘çŸ¥é“ï¼Œæ‰€ä»¥æˆ‘æ›´åº”è¯¥å»ä¼šä¼šä»–ä»¬ã€‚";
	return action;
end

function Task_00080108_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘è¿˜çœŸä¸èƒ½å°çœ‹ä½ ï¼Œå»å§ï¼Œå¹³å®‰å›æ¥ã€‚";
	action.m_ActionMsg = "æ”¾å¿ƒï¼Œæˆ‘ä¸æ˜¯æ‡¦å¼±ä¹‹äººã€‚";
	return action;
end

function Task_00080108_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘æœ‰å¥–èµç»™ä½ ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080108_step_table = {
		[1] = Task_00080108_step_01,
		[2] = Task_00080108_step_02,
		[10] = Task_00080108_step_10,
		};

function Task_00080108_step(step)
	if Task_00080108_step_table[step] ~= nil then
		return Task_00080108_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080108_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080108() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80108) then
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
function Task_00080108_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80108) then
		return false;
	end


	player:AddExp(150000);
	player:getCoin(120000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080108_abandon()
	local task = GetPlayer():GetTaskMgr();
	if task:AbandonTask(80108) then
		if not task:TaskExist(80109) and not task:TaskExist(80110) then
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
