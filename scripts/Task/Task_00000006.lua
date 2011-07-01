--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000006()
	local player = GetPlayer();
	if player:GetLev() < 6 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(6) or task:HasCompletedTask(6) or task:HasSubmitedTask(6) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(5) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(5) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000006()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 6 then
		return false;
	end
	if task:HasAcceptedTask(6) or task:HasCompletedTask(6) or task:HasSubmitedTask(6) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(5) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(5) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000006()
	if GetPlayer():GetTaskMgr():HasCompletedTask(6) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000006(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(6) == npcId and Task_Accept_00000006 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 6
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "é˜µæ³•å›¾å†Œ";
	elseif task:GetTaskSubmitNpc(6) == npcId then
		if Task_Submit_00000006() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 6
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "é˜µæ³•å›¾å†Œ";
		elseif task:HasAcceptedTask(6) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 6
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "é˜µæ³•å›¾å†Œ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000006_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å’¦ï¼Œè¿™å‡ æœ¬å¤ç±åœ¨æˆ‘è¿™é‡ŒçœŸæ˜¯æ˜ç è’™å°˜äº†ï¼Œè¿™æœ¬é˜µæ³•å›¾å†Œçœ‹èµ·æ¥ç„å¥¥æ— æ¯”ï¼Œå°ç”Ÿæˆ‘æ˜¯è‚‰çœ¼å‡¡èƒè‡ªç„¶ä¸äº†è§£ä½ ä»¬å‰‘ä¾ ä¹‹æµâ€¦â€¦ä¸è¿‡å‘¨å¤§å“¥å©å’çš„äº‹æƒ…æˆ‘ä¸€å®šå¦¥å–„å®‰æ’ï¼Œä½ å›å»å‘Šè¯‰å‘¨å¤§å“¥ä¸€å£°ï¼Œè®©ä»–æ”¾å¿ƒå°±æ˜¯ã€‚";
	action.m_ActionMsg = "å¥½ï¼Œæˆ‘è¿™å°±å»è½¬å‘Šä»–ã€‚";
	return action;
end

function Task_00000006_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¦‚æ­¤ï¼Œæˆ‘å°±æ”¾å¿ƒäº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000006_step_table = {
		[1] = Task_00000006_step_01,
		[10] = Task_00000006_step_10,
		};

function Task_00000006_step(step)
	if Task_00000006_step_table[step] ~= nil then
		return Task_00000006_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000006_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000006() then
		return false;
	end
	if not task:AcceptTask(6) then
		return false;
	end
	task:AddTaskStep(6);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000006_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(6) then
		return false;
	end


	player:AddExp(2000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000006_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(6);
end
