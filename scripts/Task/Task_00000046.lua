--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000046()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(46) or task:HasCompletedTask(46) or task:HasSubmitedTask(46) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(45) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(45) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000046()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(46) or task:HasCompletedTask(46) or task:HasSubmitedTask(46) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(45) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(45) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000046()
	if GetPlayer():GetTaskMgr():HasCompletedTask(46) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000046(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(46) == npcId and Task_Accept_00000046 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 46
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "åœ°ç‰¢åƒ§å…µ";
	elseif task:GetTaskSubmitNpc(46) == npcId then
		if Task_Submit_00000046() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 46
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "åœ°ç‰¢åƒ§å…µ";
		elseif task:HasAcceptedTask(46) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 46
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "åœ°ç‰¢åƒ§å…µ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000046_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "çœŸæ˜¯è°¢å¤©è°¢åœ°ï¼Œæˆ‘è¢«å…³äº†æ•°æ—¥ï¼Œæ²¡æƒ³åˆ°è¿˜å¯ä»¥é‡è§å¤©æ—¥ã€‚ä¸è¿‡è¿™ä¸ªåœ°ç‰¢å®ˆå«æ£®ä¸¥ï¼Œæ—¶å¸¸æœ‰åƒ§å…µå®ˆå«æ¥å·¡é€»ï¼Œè‹¥æ˜¯å‘ç°æˆ‘å¤±è¸ªï¼Œææ€•ä¼šé¸£é’ŸæŠ¥è­¦ï¼Œå¿…é¡»å…ˆé™¤æ‰è¿™äº›åƒ§å…µå®ˆå«ã€‚";
	action.m_ActionMsg = "å¥½çš„ï¼Œä½ åƒä¸‡å°å¿ƒï¼Œæˆ‘å¾ˆå¿«å°±å›æ¥ã€‚";
	return action;
end

function Task_00000046_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ æ•‘å‘½ä¹‹æ©ï¼Œæˆ‘å‘¨äº‘ä»æ²¡é½¿éš¾å¿˜ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000046_step_table = {
		[1] = Task_00000046_step_01,
		[10] = Task_00000046_step_10,
		};

function Task_00000046_step(step)
	if Task_00000046_step_table[step] ~= nil then
		return Task_00000046_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000046_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000046() then
		return false;
	end
	if not task:AcceptTask(46) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000046_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(46) then
		return false;
	end


	player:AddExp(3333);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000046_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(46);
end
