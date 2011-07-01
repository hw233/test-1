--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000104()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(104) or task:HasCompletedTask(104) or task:HasSubmitedTask(104) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000104()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(104) or task:HasCompletedTask(104) or task:HasSubmitedTask(104) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000104()
	if GetPlayer():GetTaskMgr():HasCompletedTask(104) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000104(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(104) == npcId and Task_Accept_00000104 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 104
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ä¸ºæ¶å‰‘ä¾ ";
	elseif task:GetTaskSubmitNpc(104) == npcId then
		if Task_Submit_00000104() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 104
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ä¸ºæ¶å‰‘ä¾ ";
		elseif task:HasAcceptedTask(104) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 104
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ä¸ºæ¶å‰‘ä¾ ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000104_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä¹‹å‰å¬è¯´æ…ˆäº‘å¯ºæ¥äº†ä¸å°‘æ¶äººï¼Œä»Šæ—¥è§ç™¾å§“æ¥å‘ŠçŠ¶æ‰å‘è§‰ç«Ÿæ˜¯çœŸäº‹ï¼Œæœ‰ä¸€ä¸ªå«æ¯›å¤ªçš„å®¶ä¼™æ—¶å¸¸æ¥éƒŠå¤–éªšæ‰°è‰¯å®¶å¦‡å¥³ï¼Œè¿˜å¸Œæœ›<name>å»å°†è¿™ä¸ªæ¯›å¤ªé™¤æ‰ã€‚";
	action.m_ActionMsg = "ä¸ºæ°‘é™¤å®³æ˜¯æˆ‘è¾ˆæœ¬è‰²ï¼Œå°å¯å»å»å°±å›ã€‚";
	return action;
end

function Task_00000104_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "<name>ä½ çœŸæ˜¯èº«æ‰‹ä¸å‡¡å•Šã€‚ ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000104_step_table = {
		[1] = Task_00000104_step_01,
		[10] = Task_00000104_step_10,
		};

function Task_00000104_step(step)
	if Task_00000104_step_table[step] ~= nil then
		return Task_00000104_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000104_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000104() then
		return false;
	end
	if not task:AcceptTask(104) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000104_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(104) then
		if DayTaskAward(0) then
			task:DayTaskSubmit(104);
			return true;
		end
	end

	return false;
end

--·ÅÆúÈÎÎñ
function Task_00000104_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(104);
end
