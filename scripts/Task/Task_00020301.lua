--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00020301()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 19 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20301) or task:HasCompletedTask(20301) or task:HasSubmitedTask(20301) then
		return false;
	end
	if not task:HasSubmitedTask(20206) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00020301()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 19 then
		return false;
	end
	if task:HasAcceptedTask(20301) or task:HasCompletedTask(20301) or task:HasSubmitedTask(20301) then
		return false;
	end
	if not task:HasSubmitedTask(20206) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00020301()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20301) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00020301(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20301) == npcId and Task_Accept_00020301 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20301
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¯»æ‰¾çŸ³ç¢‘";
	elseif task:GetTaskSubmitNpc(20301) == npcId then
		if Task_Submit_00020301() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20301
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¯»æ‰¾çŸ³ç¢‘";
		elseif task:HasAcceptedTask(20301) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20301
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¯»æ‰¾çŸ³ç¢‘";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00020301_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "å‹‡å£«ä½ åœ¨æ¥æ•¦ç…ŒåŸçš„è·¯ä¸Šæœ‰æ²¡æœ‰æ³¨æ„åˆ°ä»€ä¹ˆå•Šï¼Ÿ";
	action.m_ActionMsg = "æ²¡æœ‰æ³¨æ„åˆ°ï¼Œè¿˜è¯·æ˜ç¤ºã€‚";
	return action;
end

function Task_00020301_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‡ºäº†æ•¦ç…ŒåŸå¾€ä¸œï¼Œä½ å†å›å»æ‰¾æ‰¾çœ‹å§ã€‚";
	action.m_ActionMsg = "å¥½çš„ï¼Œæˆ‘ç°åœ¨å°±å»æ•¦ç…ŒåŸæ‰¾æ‰¾çœ‹ã€‚";
	return action;
end

function Task_00020301_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‹‡å£«ä½ åœ¨æ¥æ•¦ç…ŒåŸçš„è·¯ä¸Šæœ‰æ²¡æœ‰æ³¨æ„åˆ°ä»€ä¹ˆå•Šï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020301_step_table = {
		[1] = Task_00020301_step_01,
		[2] = Task_00020301_step_02,
		[10] = Task_00020301_step_10,
		};

function Task_00020301_step(step)
	if Task_00020301_step_table[step] ~= nil then
		return Task_00020301_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00020301_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020301() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20301) then
		return false;
	end
	task:AddTaskStep(20301);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00020301_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(20301) then
		return false;
	end


	player:AddExp(1800);
	player:getCoin(1700);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00020301_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20301);
end
