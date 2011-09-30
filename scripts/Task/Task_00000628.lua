--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000628()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(628) or task:HasCompletedTask(628) or task:HasSubmitedTask(628) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000628()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(628) or task:HasCompletedTask(628) or task:HasSubmitedTask(628) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000628()
	if GetPlayer():GetTaskMgr():HasCompletedTask(628) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000628(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(628) == npcId and Task_Accept_00000628 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 628
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ä½œæ¶å¤šç«¯";
	elseif task:GetTaskSubmitNpc(628) == npcId then
		if Task_Submit_00000628() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 628
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ä½œæ¶å¤šç«¯";
		elseif task:HasAcceptedTask(628) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 628
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
function Task_00000628_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆéƒ½å‰æ®µæ—¶é—´æ²»å®‰çœŸå¾—å¾ˆæˆé—®é¢˜ï¼Œæœ‰ä¸€äº›æ…ˆäº‘å¯ºå‡ºæ¥çš„å¦–äººåœ¨é™„è¿‘ä¸ºæ¶ï¼Œç¥¸å®³ç™¾å§“ï¼Œä»–ä»¬éƒ½æ˜¯ä¼ è¯´ä¸­çš„å‰‘ä»™ä¸€æµï¼Œæˆ‘ä»¬æ™®é€šæ•å¿«å¥ˆä½•ä¸äº†ä»–ä»¬ï¼Œè¿˜å¸Œæœ›"..GetPlayerName(GetPlayer()).."å»å°†è¿™äº›æ¶äººé™¤æ‰ã€‚";
	action.m_ActionMsg = "ä¸ºæ°‘é™¤å®³æ˜¯æˆ‘è¾ˆæœ¬è‰²ï¼Œå°å¯å»å»å°±å›ã€‚";
	return action;
end

function Task_00000628_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ çœŸæ˜¯èº«æ‰‹ä¸å‡¡å•Šã€‚ ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000628_step_table = {
		[1] = Task_00000628_step_01,
		[10] = Task_00000628_step_10,
		};

function Task_00000628_step(step)
	if Task_00000628_step_table[step] ~= nil then
		return Task_00000628_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000628_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000628() then
		return false;
	end
	if not task:AcceptTask(628) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000628_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(628) then
		return false;
	end


	player:AddExp(3000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000628_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(628);
end
