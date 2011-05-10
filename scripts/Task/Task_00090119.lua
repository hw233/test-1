--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090119()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90119) or task:HasCompletedTask(90119) or task:HasSubmitedTask(90119) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090119()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90119) or task:HasCompletedTask(90119) or task:HasSubmitedTask(90119) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090119()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90119) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090119(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90119) == npcId and Task_Accept_00090119 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90119
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç¾é£Ÿ";
	elseif task:GetTaskSubmitNpc(90119) == npcId then
		if Task_Submit_00090119() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90119
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç¾é£Ÿ";
		elseif task:HasAcceptedTask(90119) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90119
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç¾é£Ÿ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090119_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "å‹‡å£«ï¼Œä½ å¸®æˆ‘è·‘ä¸€è¶Ÿå§ã€‚";
	action.m_ActionMsg = "è¿™å›ä¸ä¼šæ˜¯åˆè¦å»æ¢æœ›è°å§ï¼Ÿ";
	return action;
end

function Task_00090119_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä¸æ˜¯ï¼Œæˆ‘æƒ³è®©ä½ å»æ‰¾å¤©æ± çš„è·¯ä¹‹é¥ï¼Œè®©ä»–åšäº›ç¾é£Ÿé€æ¥å¤œæ‘©åŸçŠ’åŠ³ä½œæˆ˜çš„å°†å£«ä»¬ï¼Œå¤§å®¶éƒ½éœ€è¦è¡¥å……ä½“åŠ›ã€‚";
	action.m_ActionMsg = "çœ‹æ¥è·¯ä¹‹é¥çš„æ‰‹è‰ºæŒºä¸é”™çš„å‘€ï¼Œå¤§å®¶éƒ½å–œæ¬¢åƒä»–åšçš„é£Ÿç‰©ã€‚";
	return action;
end

function Task_00090119_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘çš„æ‰‹è‰ºè¶…çº§æ£’ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090119_step_table = {
		[1] = Task_00090119_step_01,
		[2] = Task_00090119_step_02,
		[10] = Task_00090119_step_10,
		};

function Task_00090119_step(step)
	if Task_00090119_step_table[step] ~= nil then
		return Task_00090119_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090119_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090119() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90119) then
		return false;
	end
	task:AddTaskStep(90119);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090119_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90119) then
		return false;
	end


	player:AddExp(650000);
	player:getCoin(312000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090119_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90119);
end
