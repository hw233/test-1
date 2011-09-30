--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000639()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(639) or task:HasCompletedTask(639) or task:HasSubmitedTask(639) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000639()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(639) or task:HasCompletedTask(639) or task:HasSubmitedTask(639) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000639()
	if GetPlayer():GetTaskMgr():HasCompletedTask(639) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000639(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(639) == npcId and Task_Accept_00000639 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 639
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¦–åŒ–æ‘æ°‘";
	elseif task:GetTaskSubmitNpc(639) == npcId then
		if Task_Submit_00000639() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 639
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¦–åŒ–æ‘æ°‘";
		elseif task:HasAcceptedTask(639) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 639
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¦–åŒ–æ‘æ°‘";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000639_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é•¿æ²™éƒŠå¤–æœ‰ä¸€äº›æ¶äººï¼Œå—äº†é­”æ•™ä¸­äººçš„è›Šæƒ‘ï¼Œåœ¨é•¿æ²™ä¸ºæ¶ï¼Œä»–ä»¬è¯·æ¥äº†ä¸€ä¸ªå¦–é“ä½¿ç”¨é‚ªæ³•å°†å½“åœ°å–„è‰¯çš„æ‘æ°‘è½¬åŒ–ä¸ºå‡¶å…½ï¼Œä¸ºæ¶ä¸€æ–¹ï¼Œ"..GetPlayerName(GetPlayer()).."ä½ é€Ÿé€Ÿå»é™¤æ‰è¿™äº›å¦–åŒ–æ‘æ°‘ã€‚";
	action.m_ActionMsg = "å‡ ä¸ªé‚ªç‰©ä¸åœ¨è¯ä¸‹ï¼Œæˆ‘å»å»å°±å›ã€‚";
	return action;
end

function Task_00000639_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ æœç„¶æ˜¯èº«æ‰‹ä¸å‡¡ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000639_step_table = {
		[1] = Task_00000639_step_01,
		[10] = Task_00000639_step_10,
		};

function Task_00000639_step(step)
	if Task_00000639_step_table[step] ~= nil then
		return Task_00000639_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000639_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000639() then
		return false;
	end
	if not task:AcceptTask(639) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000639_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(639) then
		return false;
	end


	player:AddExp(3000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000639_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(639);
end
