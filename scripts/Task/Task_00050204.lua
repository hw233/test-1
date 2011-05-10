--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00050204()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 20 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50204) or task:HasCompletedTask(50204) or task:HasSubmitedTask(50204) then
		return false;
	end
	if not task:HasSubmitedTask(50203) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00050204()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 20 then
		return false;
	end
	if task:HasAcceptedTask(50204) or task:HasCompletedTask(50204) or task:HasSubmitedTask(50204) then
		return false;
	end
	if not task:HasSubmitedTask(50203) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00050204()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50204) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00050204(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50204) == npcId and Task_Accept_00050204 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50204
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ‹›å‹Ÿç”°å¿Œ";
	elseif task:HasAcceptedTask(50204) and npcId == 137 then
		RunAutoBattleAction(player, 137, -1);
	elseif task:GetTaskSubmitNpc(50204) == npcId and Task_Submit_00050204() then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50204
		action.m_ActionToken = 2;
		action.m_ActionStep = 10;
		action.m_ActionMsg = "æ‹›å‹Ÿç”°å¿Œ";
	end	
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00050204_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ›¿æˆ‘è°¢è°¢ç‹å’Œç‹åï¼Œå¥¹å¸Œæœ›æˆ‘è¿˜èƒ½ç»§ç»­ä¸ºé­”æ—æ•ˆåŠ›ã€‚å‹‡å£«ï¼Œå¦‚æœä½ èƒ½å‡»è´¥æˆ‘ï¼Œæˆ‘å°†æˆä¸ºä½ çš„å°†é¢†ï¼Œéšä½ ä¸€èµ·å¾æˆ˜è´ªå©ªçš„å¤©æ—ã€‚";
	action.m_ActionMsg = "æˆ‘ä¼šç”¨å®åŠ›å‘ä½ è¯æ˜ï¼Œæˆ‘æ˜¯ä¸€åçœŸæ­£çš„å‹‡å£«ã€‚";
	return action;
end

function Task_00050204_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä»Šåè¯·é˜ä¸‹å¤šå¤šæŒ‡ç‚¹äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050204_step_table = {
		[1] = Task_00050204_step_01,
		[10] = Task_00050204_step_10,
		};

function Task_00050204_step(step)
	if Task_00050204_step_table[step] ~= nil then
		return Task_00050204_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00050204_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050204() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50204) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00050204_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if player:isFighterFull() then
		player:sendMsgCode(0, 2006);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(50204) then
		return false;
	end


	player:AddExp(2000);
	player:getCoin(10000);
	player:getTael(120);
	player:takeFighter(137, true);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00050204_abandon()
	return GetPlayer():GetTaskMgr():AbandonTask(50204);
end
