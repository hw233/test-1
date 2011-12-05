--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000151()
	local player = GetPlayer();
	if player:GetLev() < 70 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(151) or task:HasCompletedTask(151) or task:HasSubmitedTask(151) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(150) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(150) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(150) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000151()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 70 then
		return false;
	end
	if task:HasAcceptedTask(151) or task:HasCompletedTask(151) or task:HasSubmitedTask(151) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(150) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(150) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(150) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000151()
	if GetPlayer():GetTaskMgr():HasCompletedTask(151) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000151(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(151) == npcId and Task_Accept_00000151 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 151
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "è½æ¹–é•‡";
	elseif task:GetTaskSubmitNpc(151) == npcId then
		if Task_Submit_00000151() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 151
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "è½æ¹–é•‡";
		elseif task:HasAcceptedTask(151) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 151
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "è½æ¹–é•‡";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000151_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "åœ¨æˆ‘é™¤å¦–é²§çš„è¿™æ®µæ—¶é—´ï¼Œé›è¡å±±è„šè½æ¹–é•‡çš„æ‘æ°‘å¯¹æˆ‘ç¤¼é‡æœ‰åŠ ï¼Œåªæ˜¯æ©å¸ˆæœ‰å‘½ï¼Œè¦æˆ‘é™¤å¦–åå³åˆ»å›ç™¾èŠ±å±±ã€‚è¿™é›è¡é è¿‘ä¸œæµ·ï¼Œä¹Ÿæ»‹ç”Ÿäº†ä¸å°‘å¦–å­½ï¼Œæˆ‘æš‚æ—¶åˆ†èº«ä¹æœ¯ï¼Œè¿˜å¸Œæœ›ä½ å¯ä»¥å»å¸®åŠ©ä¸‡æ‘é•¿è§£å†³å‘¨é­çš„ç¥¸å®³ã€‚";
	action.m_ActionMsg = "æ²¡é—®é¢˜â€¦â€¦";
	return action;
end

function Task_00000151_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "çœŸæ˜¯è°¢å¤©è°¢åœ°ï¼Œæœ‰å°‘ä¾ ä½ æ¥å¸®åŠ©æˆ‘ä»¬ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000151_step_table = {
		[1] = Task_00000151_step_01,
		[10] = Task_00000151_step_10,
		};

function Task_00000151_step(step)
	if Task_00000151_step_table[step] ~= nil then
		return Task_00000151_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000151_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000151() then
		return false;
	end
	if not task:AcceptTask(151) then
		return false;
	end
	task:AddTaskStep(151);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000151_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(151) then
		return false;
	end


	player:AddExp(60000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000151_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(151);
end
