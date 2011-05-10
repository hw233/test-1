--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00050409()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50409) or task:HasCompletedTask(50409) or task:HasSubmitedTask(50409) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00050409()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(50409) or task:HasCompletedTask(50409) or task:HasSubmitedTask(50409) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00050409()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50409) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00050409(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50409) == npcId and Task_Accept_00050409 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50409
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ”¶æœåå°†";
	elseif task:GetTaskSubmitNpc(50409) == npcId then
		if Task_Submit_00050409() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50409
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ”¶æœåå°†";
		elseif task:HasAcceptedTask(50409) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50409
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ”¶æœåå°†";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00050409_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ï¼Œè¾›è‹¦äº†ï¼Œå†ç»ƒçš„è·¯ç¨‹æ˜¯è‰°è¾›çš„ï¼Œä½ éœ€è¦ç»™è‡ªå·±å¢å¼ºæ­¦åŠ›ï¼Œå»é…’é¦†å¯»æ‰¾ä½ çš„å°†é¢†å§ï¼Œå½“ç„¶ï¼Œä½ å¾—èŠ±ç‚¹é’±ï¼Œæ‰èƒ½æ‰¾åˆ°å¥½å°†é¢†ã€‚";
	action.m_ActionMsg = "å¥½çš„ï¼Œæˆ‘è¿™å°±å»ã€‚";
	return action;
end

function Task_00050409_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ æ˜¯æˆ‘ä»¬é­”æ—æœ€å¼ºå£®çš„å‹‡å£«ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050409_step_table = {
		[1] = Task_00050409_step_01,
		[10] = Task_00050409_step_10,
		};

function Task_00050409_step(step)
	if Task_00050409_step_table[step] ~= nil then
		return Task_00050409_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00050409_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050409() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50409) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00050409_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(50409) then
		return false;
	end


	player:AddExp(3000);
	player:getCoin(10000);
	player:getTael(120);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00050409_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50409);
end
