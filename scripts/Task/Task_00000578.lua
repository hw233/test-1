--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000578()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(578) or task:HasCompletedTask(578) or task:HasSubmitedTask(578) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000578()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(578) or task:HasCompletedTask(578) or task:HasSubmitedTask(578) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000578()
	if GetPlayer():GetTaskMgr():HasCompletedTask(578) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000578(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(578) == npcId and Task_Accept_00000578 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 578
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ä½œæ¶å¤šç«¯";
	elseif task:GetTaskSubmitNpc(578) == npcId then
		if Task_Submit_00000578() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 578
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ä½œæ¶å¤šç«¯";
		elseif task:HasAcceptedTask(578) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 578
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ä½œæ¶å¤šç«¯";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000578_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ…ˆäº‘å¯ºæœ€è¿‘èšé›†äº†ä¸å°‘é‚ªæ´¾äººå£«ï¼Œæœ‰ä¸€äº›å¦–äººåœ¨é™„è¿‘ä¸ºæ¶ï¼Œç¥¸å®³ç™¾å§“ï¼Œå®åœ¨æ˜¯æˆ‘ä»¬å‰‘ä¾ é‡Œçš„è´¥ç±»ï¼Œ"..GetPlayerName(GetPlayer()).."ä½ é€Ÿé€Ÿå»å°†è¿™äº›æ¶äººé™¤æ‰ã€‚";
	action.m_ActionMsg = "ä¸ºæ°‘é™¤å®³æ˜¯æˆ‘è¾ˆæœ¬è‰²ï¼Œå¼Ÿå­å»å»å°±å›ã€‚";
	return action;
end

function Task_00000578_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ çœŸæ˜¯èº«æ‰‹ä¸å‡¡å•Šã€‚ ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000578_step_table = {
		[1] = Task_00000578_step_01,
		[10] = Task_00000578_step_10,
		};

function Task_00000578_step(step)
	if Task_00000578_step_table[step] ~= nil then
		return Task_00000578_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000578_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000578() then
		return false;
	end
	if not task:AcceptTask(578) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000578_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(578) then
		return false;
	end


	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000578_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(578);
end
