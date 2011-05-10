--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090117()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90117) or task:HasCompletedTask(90117) or task:HasSubmitedTask(90117) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090117()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90117) or task:HasCompletedTask(90117) or task:HasSubmitedTask(90117) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090117()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90117) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090117(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90117) == npcId and Task_Accept_00090117 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90117
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å›ç»å‹’å‹¤";
	elseif task:GetTaskSubmitNpc(90117) == npcId then
		if Task_Submit_00090117() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90117
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å›ç»å‹’å‹¤";
		elseif task:HasAcceptedTask(90117) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90117
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å›ç»å‹’å‹¤";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090117_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "æˆ‘çŸ¥é“ä½ æ¥è¿™é‡ŒåŠå•å¸ƒå¾ˆä¹…äº†ï¼Œæˆ‘å¸Œæœ›ä½ èƒ½å»å¤œæ‘©åŸå›ç»å‹’å‹¤ï¼Œå•å¸ƒæ˜¯ä¸ä¼šå»é‚£é‡Œçš„ã€‚";
	action.m_ActionMsg = "éš¾é“å•å…ˆç”Ÿå’Œå‹’å°†å†›ä¸åˆï¼Ÿ";
	return action;
end

function Task_00090117_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ—§äº‹è«æï¼Œå¸¸æ³‰å¡ç¡®å®éœ€è¦å•å¸ƒåœ¨è¿™é•‡å®ˆã€‚";
	action.m_ActionMsg = "é‚£å¥½å§ï¼Œæˆ‘æƒ³å‹’å°†å†›è¦å¤±æœ›äº†ã€‚";
	return action;
end

function Task_00090117_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "çœ‹æ¥ä»–è¿˜æ˜¯å¿˜ä¸äº†æ—§äº‹ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090117_step_table = {
		[1] = Task_00090117_step_01,
		[2] = Task_00090117_step_02,
		[10] = Task_00090117_step_10,
		};

function Task_00090117_step(step)
	if Task_00090117_step_table[step] ~= nil then
		return Task_00090117_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090117_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090117() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90117) then
		return false;
	end
	task:AddTaskStep(90117);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090117_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90117) then
		return false;
	end


	player:AddExp(650000);
	player:getCoin(312000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090117_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90117);
end
