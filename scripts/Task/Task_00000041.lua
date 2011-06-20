--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000041()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(41) or task:HasCompletedTask(41) or task:HasSubmitedTask(41) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(40) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(40) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000041()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(41) or task:HasCompletedTask(41) or task:HasSubmitedTask(41) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(40) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(40) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000041()
	if GetPlayer():GetTaskMgr():HasCompletedTask(41) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000041(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(41) == npcId and Task_Accept_00000041 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 41
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¼ºåŒ–æ³•å®";
	elseif task:GetTaskSubmitNpc(41) == npcId then
		if Task_Submit_00000041() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 41
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¼ºåŒ–æ³•å®";
		elseif task:HasAcceptedTask(41) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 41
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¼ºåŒ–æ³•å®";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000041_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æƒ³ä¸åˆ°é†‰é“äººè¿™ä¸ªæ•´å¤©æ³¡åœ¨é…’è‘«èŠ¦é‡Œçš„å®¶ä¼™ï¼Œç«Ÿç„¶è€çœ¼ä¸èŠ±ï¼Œæ‰¾åˆ°è±¡ä½ èµ„è´¨è¿™ä¹ˆé«˜çš„å¹´é’äººâ€¦â€¦ä½ çš„æ¥æ„æˆ‘å·²çŸ¥æ™“ï¼Œä»Šå¤©å—å°±å…è´¹æ•™ä½ å¼ºåŒ–æ³•å®ï¼Œè¦çŸ¥é“æˆ‘æ”¶è´¹å¯æ˜¯å¾ˆè´µçš„å“¦ã€‚";
	action.m_ActionMsg = "å¤šè°¢å»–å‰è¾ˆäº†ã€‚";
	return action;
end

function Task_00000041_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘å°±çŸ¥é“è¿™å®¶ä¼™æœ€å–œçˆ±é‚£äº›ä»™æ ¹ç¦åšï¼Œèµ„è´¨ä¸å‡¡çš„å¹´é’äººäº†ï¼Œçœä¸‹è¿™ç‚¹é“¶å­åˆå¤Ÿæˆ‘å–å‡ å¤©é…’äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000041_step_table = {
		[1] = Task_00000041_step_01,
		[10] = Task_00000041_step_10,
		};

function Task_00000041_step(step)
	if Task_00000041_step_table[step] ~= nil then
		return Task_00000041_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000041_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000041() then
		return false;
	end
	if not task:AcceptTask(41) then
		return false;
	end
	task:AddTaskStep(41);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000041_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(41) then
		return false;
	end


	player:AddExp(1111);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000041_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(41);
end
