--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090108()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90108) or task:HasCompletedTask(90108) or task:HasSubmitedTask(90108) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90107) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90107) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090108()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90108) or task:HasCompletedTask(90108) or task:HasSubmitedTask(90108) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90107) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90107) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090108()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90108) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090108(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90108) == npcId and Task_Accept_00090108 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90108
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æµè¡€çš„éª·é«…";
	elseif task:GetTaskSubmitNpc(90108) == npcId then
		if Task_Submit_00090108() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90108
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æµè¡€çš„éª·é«…";
		elseif task:HasAcceptedTask(90108) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90108
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æµè¡€çš„éª·é«…";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090108_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "çœŸçš„æ˜¯é‚£äº›æ€ªç‰©åœ¨å®³æˆ‘å—ï¼Ÿ";
	action.m_ActionMsg = "æ˜¯çš„ï¼Œä¸è¿‡æ˜¯äº›å°å¦–æ€ªï¼Œä½ æ— éœ€å®³æ€•çš„ã€‚";
	return action;
end

function Task_00090108_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ èƒ½å¸®æˆ‘å»æ€äº†é‚£äº›éª·é«…å—ï¼Ÿé‚£äº›åŠ¨é™æ€»æ˜¯è®©æˆ‘æ— æ³•å®‰ç¡ã€‚";
	action.m_ActionMsg = "çœ‹æ¥é‚£äº›æ€ªç‰©ç€å®å½±å“åˆ°ä½ äº†ï¼Œé‚£æˆ‘å»å¸®ä½ è§£å†³å§ã€‚";
	return action;
end

function Task_00090108_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘ç»ˆäºå¯ä»¥æ¸…é™çš„ç¡ä¸Šä¸€è§‰äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090108_step_table = {
		[1] = Task_00090108_step_01,
		[2] = Task_00090108_step_02,
		[10] = Task_00090108_step_10,
		};

function Task_00090108_step(step)
	if Task_00090108_step_table[step] ~= nil then
		return Task_00090108_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090108_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090108() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90108) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090108_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90108) then
		return false;
	end


	player:AddExp(575000);
	player:getCoin(306000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090108_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90108);
end
