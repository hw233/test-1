--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000098()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(98) or task:HasCompletedTask(98) or task:HasSubmitedTask(98) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(97) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(97) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(97) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000098()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(98) or task:HasCompletedTask(98) or task:HasSubmitedTask(98) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(97) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(97) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(97) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000098()
	if GetPlayer():GetTaskMgr():HasCompletedTask(98) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000098(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(98) == npcId and Task_Accept_00000098 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 98
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "èµ¤å‘å¯¨ä¸»";
	elseif task:GetTaskSubmitNpc(98) == npcId then
		if Task_Submit_00000098() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 98
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "èµ¤å‘å¯¨ä¸»";
		elseif task:HasAcceptedTask(98) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 98
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "èµ¤å‘å¯¨ä¸»";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000098_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™èµ¤å‘å¯¨ä¸»è™½æ˜¯å·¦æ´¾ä¸­äººï¼Œä½†ä¹‹å‰ä¹Ÿæœªä½œæ¶å¤ªå¤šï¼Œæˆ‘å¬è¯´ä»–ä¸çŸ¥ä»å“ªé‡Œå­¦ä¼šä¸€ç§é­”åŠŸï¼Œä¿®ç‚¼çš„å¿ƒæ€§å¤§å˜ï¼Œè¡Œäº‹ä¹Ÿè¶Šå‘é‚ªæ¶ã€‚å¦‚ä»Šä¹‹è®¡åªæœ‰å°†ä»–å°½æ—©é“²é™¤ï¼Œè‹¥å¾…ä»–é­”åŠŸæœ‰æˆï¼Œé‚£å°±å¤§å¤§çš„ä¸å¦™äº†ã€‚";
	action.m_ActionMsg = "å¸ˆä½ è¯´çš„æœ‰ç†ã€‚";
	return action;
end

function Task_00000098_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ‹”é™¤äº†å§‘å©†å²­çš„è¿™ä¸ªæ¯’ç˜¤ï¼Œæˆ‘ä¹Ÿå¯ä»¥å®‰å¿ƒæ½œä¿®äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000098_step_table = {
		[1] = Task_00000098_step_01,
		[10] = Task_00000098_step_10,
		};

function Task_00000098_step(step)
	if Task_00000098_step_table[step] ~= nil then
		return Task_00000098_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000098_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000098() then
		return false;
	end
	if not task:AcceptTask(98) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000098_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(98) then
		return false;
	end


	player:AddExp(20000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000098_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(98);
end
