--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090116()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90116) or task:HasCompletedTask(90116) or task:HasSubmitedTask(90116) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90115) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90115) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090116()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90116) or task:HasCompletedTask(90116) or task:HasSubmitedTask(90116) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90115) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90115) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090116()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90116) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090116(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90116) == npcId and Task_Accept_00090116 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90116
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ¸…é™¤äº¡é­‚";
	elseif task:GetTaskSubmitNpc(90116) == npcId then
		if Task_Submit_00090116() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90116
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ¸…é™¤äº¡é­‚";
		elseif task:HasAcceptedTask(90116) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90116
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ¸…é™¤äº¡é­‚";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090116_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¥½é…’ä¸æ€•å··å­æ·±ï¼Œé•¿è€…çš„ä¸€ç•ªå¥½æ„æˆ‘å¿ƒé¢†äº†ã€‚æœ€è¿‘é‚£äº›å¤ä»‡æ€¨çµæ€»æ˜¯åµé—¹ä¸ä¼‘ï¼Œé•¿è€…å¾ˆæ˜¯çƒ¦æ¼ã€‚ä½ å»å°†é‚£äº›æ€¨çµæ€æ‰ï¼Œè®©é•¿è€…æ¸…é™æ¸…é™ï¼Œå®Œäº‹å„¿åå»å‘Šè¯‰é•¿è€…ã€‚";
	action.m_ActionMsg = "çœ‹æ¥ç°åœ¨å“ªå„¿éƒ½æ˜¯ä¸å¾—å®‰å®çš„ã€‚";
	return action;
end

function Task_00090116_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è€å¤«ç»ˆäºå¯ä»¥æ¸…é™æ¸…é™äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090116_step_table = {
		[1] = Task_00090116_step_01,
		[10] = Task_00090116_step_10,
		};

function Task_00090116_step(step)
	if Task_00090116_step_table[step] ~= nil then
		return Task_00090116_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090116_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090116() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90116) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090116_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90116) then
		return false;
	end


	player:AddExp(625000);
	player:getCoin(310000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090116_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90116);
end
