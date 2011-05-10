--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00040303()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40303) or task:HasCompletedTask(40303) or task:HasSubmitedTask(40303) then
		return false;
	end
	if not task:HasSubmitedTask(40203) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00040303()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(40303) or task:HasCompletedTask(40303) or task:HasSubmitedTask(40303) then
		return false;
	end
	if not task:HasSubmitedTask(40203) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00040303()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40303) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00040303(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40303) == npcId and Task_Accept_00040303() then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40303
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ‹›å‹Ÿæè‡ªæˆ";
	elseif task:HasAcceptedTask(40303) and npcId == 138 then
		RunAutoBattleAction(player, 138, -1);
	elseif task:GetTaskSubmitNpc(40303) == npcId and Task_Submit_00040303() then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40303
		action.m_ActionToken = 2;
		action.m_ActionStep = 10;
		action.m_ActionMsg = "æ‹›å‹Ÿæè‡ªæˆ";
	end
	
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00040303_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "æ‹¥æœ‰äº†åˆºå®¢å’Œè°‹å£«ä¹‹åï¼Œä½ æœ‰æ²¡æœ‰æ˜æ˜¾æ„Ÿåˆ°å®åŠ›å¤§å¢å•Šï¼Ÿè¿™å°±æ˜¯é˜Ÿä¼çš„åŠ›é‡ï¼Œå…¶å®ä»¥ä½ ç°åœ¨çš„ç»Ÿé¢†èƒ½åŠ›è¿˜å¯ä»¥å†æ·»ä¸€å‘˜ã€‚";
	action.m_ActionMsg = "æ˜¯å˜›ï¼Œè¯·ä½ æŒ‡ç‚¹æˆ‘ã€‚";
	return action;
end

function Task_00040303_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç°åœ¨çš„ä½ è¿˜ç¼ºå°‘ä¸€åçŒ›å°†ï¼Œå»è¯•ç€æŒ‘æˆ˜é•‡å‰è¥çš„æè‡ªæˆå§ã€‚";
	action.m_ActionMsg = "äº‹ä¸å®œè¿Ÿï¼Œæˆ‘ç°åœ¨å°±å»ã€‚";
	return action;
end

function Task_00040303_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "åœ¨ä¸‹æ„¿æ•ˆçŠ¬é©¬ä¹‹åŠ³ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040303_step_table = {
		[1] = Task_00040303_step_01,
		[2] = Task_00040303_step_02,
		[10] = Task_00040303_step_10,
		};

function Task_00040303_step(step)
	if Task_00040303_step_table[step] ~= nil then
		return Task_00040303_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00040303_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040303() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40303) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00040303_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	
	if player:isFighterFull() then
		player:sendMsgCode(0, 2006);
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(40303) then
		return false;
	end


	player:AddExp(3000);
	player:getCoin(10000);
	player:getTael(150);
	player:takeFighter(138, true);
	
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00040303_abandon()
	return GetPlayer():GetTaskMgr():AbandonTask(40303);
end
