--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00040305()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 35 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40305) or task:HasCompletedTask(40305) or task:HasSubmitedTask(40305) then
		return false;
	end
	if not task:HasSubmitedTask(40304) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00040305()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 35 then
		return false;
	end
	if task:HasAcceptedTask(40305) or task:HasCompletedTask(40305) or task:HasSubmitedTask(40305) then
		return false;
	end
	if not task:HasSubmitedTask(40304) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00040305()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40305) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00040305(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40305) == npcId and Task_Accept_00040305 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40305
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å®åŠ›çš„è¾ƒé‡";
	elseif task:GetTaskSubmitNpc(40305) == npcId then
		if Task_Submit_00040305() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40305
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å®åŠ›çš„è¾ƒé‡";
		elseif task:HasAcceptedTask(40305) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40305
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å®åŠ›çš„è¾ƒé‡";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00040305_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "ç°åœ¨ï¼Œä½ çœ‹åˆ°æˆ‘ä»¬å¤©æ—çš„åŠ›é‡æœ‰å¤šä¹ˆå¼ºå¤§äº†å§ã€‚";
	action.m_ActionMsg = "æ˜¯çš„ï¼Œæˆ‘è¿˜éœ€è¦åŠªåŠ›é”»ç‚¼è‡ªå·±ã€‚";
	return action;
end

function Task_00040305_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ©ï¼Œä¸è¿‡ç»è¿‡è¿™ä¸€æ¬¡ï¼Œæˆ‘å‘ç°ä½ è¿›æ­¥å¾ˆå¤§ã€‚æ‰€ä»¥ä½ æ¥ä¸‹æ¥å°†å†æ¬¡å»åˆ°ç«æŠ€åœºï¼Œå®Œæˆ3æ¬¡æŒ‘æˆ˜ï¼Œä½ å°†ä¼šæˆé•¿æ›´å¿«çš„ã€‚";
	action.m_ActionMsg = "æˆ‘ä¼šè°¨éµå¤§äººçš„æ•™è¯²ã€‚";
	return action;
end

function Task_00040305_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç»ˆæœ‰ä¸€å¤©ï¼Œä½ ä¼šæˆä¸ºæˆ‘ä»¬å¤©æ—æœ€å¼ºå¤§çš„å‹‡å£«ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040305_step_table = {
		[1] = Task_00040305_step_01,
		[2] = Task_00040305_step_02,
		[10] = Task_00040305_step_10,
		};

function Task_00040305_step(step)
	if Task_00040305_step_table[step] ~= nil then
		return Task_00040305_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00040305_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040305() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40305) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00040305_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40305) then
		return false;
	end


	player:AddExp(2000);
	player:getCoin(10000);
	player:getTael(120);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00040305_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40305);
end
