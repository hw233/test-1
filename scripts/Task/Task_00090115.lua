--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090115()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90115) or task:HasCompletedTask(90115) or task:HasSubmitedTask(90115) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90114) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90114) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090115()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90115) or task:HasCompletedTask(90115) or task:HasSubmitedTask(90115) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90114) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90114) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090115()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90115) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090115(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90115) == npcId and Task_Accept_00090115 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90115
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¥½é…’é…è‹±é›„";
	elseif task:GetTaskSubmitNpc(90115) == npcId then
		if Task_Submit_00090115() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90115
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¥½é…’é…è‹±é›„";
		elseif task:HasAcceptedTask(90115) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90115
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¥½é…’é…è‹±é›„";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090115_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "çœŸæ˜¯å¯æƒœï¼Œåªå‰©è¿™ä¸€å›äº†ã€‚";
	action.m_ActionMsg = "æˆ‘å»çš„æ—¶å€™é‚£äº›æ€ªç‰©æ­£åœ¨é¥®é…’ä½œä¹ï¼Œçœ‹æ¥è¿™é…’å‘³é“è‚¯å®šä¸é”™ã€‚";
	return action;
end

function Task_00090115_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ˜¯å‘€ï¼Œæˆ‘è´¹äº†å¾ˆå¤šå¿ƒæ€é…¿åˆ¶è¿™äº›ç¾é…’ã€‚å”‰ï¼Œé‚£è¯·å‹‡å£«å°†è¿™æœ€åä¸€å›ç¾é…’é€ä¸å•å¸ƒå–å§ã€‚";
	action.m_ActionMsg = "ä»–è‚¯å®šä¼šå¾ˆé«˜å…´çš„ã€‚";
	return action;
end

function Task_00090115_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¥½é…’ä¸æ€•å··å­æ·±ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090115_step_table = {
		[1] = Task_00090115_step_01,
		[2] = Task_00090115_step_02,
		[10] = Task_00090115_step_10,
		};

function Task_00090115_step(step)
	if Task_00090115_step_table[step] ~= nil then
		return Task_00090115_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090115_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090115() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15105, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(90115) then
		return false;
	end
	task:AddTaskStep(90115);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090115_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15105,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(90115) then
		return false;
	end

	package:DelItemAll(15105,1);

	player:AddExp(625000);
	player:getCoin(310000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090115_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15105,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90115);
end
