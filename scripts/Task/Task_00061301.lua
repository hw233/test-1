--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00061301()
	local player = GetPlayer();
	if player:GetLev() < 53 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61301) or task:HasCompletedTask(61301) or task:HasSubmitedTask(61301) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00061301()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 53 then
		return false;
	end
	if task:HasAcceptedTask(61301) or task:HasCompletedTask(61301) or task:HasSubmitedTask(61301) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00061301()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61301) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00061301(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61301) == npcId and Task_Accept_00061301 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61301
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ”¶æ‹¾æ¸”äºº";
	elseif task:GetTaskSubmitNpc(61301) == npcId then
		if Task_Submit_00061301() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61301
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ”¶æ‹¾æ¸”äºº";
		elseif task:HasAcceptedTask(61301) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61301
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ”¶æ‹¾æ¸”äºº";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00061301_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "è¯´å‹‡å£«å‹‡å£«å°±åˆ°å•Šï¼Œæˆ‘åˆšè¿˜åœ¨å¿µå¨å‘¢ï¼Œèƒ½ä¸èƒ½æ¥ä¸ªå‹‡å£«å¸®æˆ‘æŠŠé‚£äº›æ¸”äººèµ¶èµ°ï¼Œä½ å°±æ¥äº†ã€‚";
	action.m_ActionMsg = "é‚£äº›æ¸”äººæ€ä¹ˆä½ äº†ï¼Ÿ";
	return action;
end

function Task_00061301_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘æ­£å¿™ç€åˆ¶ä½œå„ç§è¯æ–¹å‘¢ï¼Œæˆ‘æ—è¾¹é‚£äº›å’¸æ°´å²¸çš„æ€’ç›®æ¸”äººï¼Œå°±ä¸€ç›´æ¥æ‰“æ‰°æˆ‘å·¥ä½œï¼Œä½ èƒ½ä¸èƒ½å»å¸®æˆ‘å§ä»–ä»¬è§£å†³æ‰å•Šã€‚";
	action.m_ActionMsg = "å—¯æ²¡é—®é¢˜ã€‚";
	return action;
end

function Task_00061301_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç°åœ¨å¥½å¤šäº†ï¼Œæˆ‘å¯ä»¥å®‰å¿ƒåˆ¶ä½œæˆ‘çš„è¯æ–¹äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061301_step_table = {
		[1] = Task_00061301_step_01,
		[2] = Task_00061301_step_02,
		[10] = Task_00061301_step_10,
		};

function Task_00061301_step(step)
	if Task_00061301_step_table[step] ~= nil then
		return Task_00061301_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00061301_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061301() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61301) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00061301_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(61301) then
		return false;
	end


	player:AddExp(68000);
	player:getCoin(57000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00061301_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61301);
end
