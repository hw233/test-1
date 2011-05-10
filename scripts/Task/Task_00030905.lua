--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00030905()
	local player = GetPlayer();
	if player:GetLev() < 55 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30905) or task:HasCompletedTask(30905) or task:HasSubmitedTask(30905) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30904) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30904) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00030905()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 55 then
		return false;
	end
	if task:HasAcceptedTask(30905) or task:HasCompletedTask(30905) or task:HasSubmitedTask(30905) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30904) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30904) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00030905()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30905) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00030905(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30905) == npcId and Task_Accept_00030905 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30905
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "é€¼ä¾›æ‘©ä¼½";
	elseif task:GetTaskSubmitNpc(30905) == npcId then
		if Task_Submit_00030905() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30905
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "é€¼ä¾›æ‘©ä¼½";
		elseif task:HasAcceptedTask(30905) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30905
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "é€¼ä¾›æ‘©ä¼½";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00030905_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "ä½ æ˜¯è°ï¼Ÿåˆ°åº•æƒ³å¹²ä»€ä¹ˆï¼Ÿ";
	action.m_ActionMsg = "æˆ‘æ˜¯è°ä¸é‡è¦ï¼Œæˆ‘å°±æ˜¯æƒ³è®©ä½ å‘Šè¯‰æˆ‘ï¼Œæ‘©å¯æ‹¿é€ƒå»å“ªé‡Œäº†ï¼Ÿ";
	return action;
end

function Task_00030905_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å•Šï¼åˆ«æ‰“äº†ï¼Œæˆ‘æ‹›ï¼Œä»–åœ¨æ‚¨è¿‡æ¥ä¹‹å‰å°±å·²ç»ä»æˆ‘è¿™é‡Œé€ƒèµ°äº†ï¼Œä¼¼ä¹æ˜¯å¾€è¥¿è¾¹å»äº†â€¦â€¦";
	action.m_ActionMsg = "ï¼ˆä¸¾èµ·æ‰‹ä¸­çš„æ­¦å™¨ï¼‰ç®—ä½ è¯†ç›¸â€¦â€¦";
	return action;
end

function Task_00030905_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "åŸæ¥æ‘©å¯æ‹¿å·²ç»å¾€è¥¿é€ƒè·‘äº†å•Šï¼Œçœ‹æ¥æˆ‘æš‚æ—¶ä¹Ÿåšä¸äº†ä»€ä¹ˆäº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030905_step_table = {
		[1] = Task_00030905_step_01,
		[2] = Task_00030905_step_02,
		[10] = Task_00030905_step_10,
		};

function Task_00030905_step(step)
	if Task_00030905_step_table[step] ~= nil then
		return Task_00030905_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00030905_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030905() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30905) then
		return false;
	end
	task:AddTaskStep(30905);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00030905_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30905) then
		return false;
	end


	player:AddExp(76000);
	player:getCoin(70000);
	player:getTael(20);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00030905_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30905);
end
