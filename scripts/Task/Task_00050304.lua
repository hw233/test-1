--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00050304()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50304) or task:HasCompletedTask(50304) or task:HasSubmitedTask(50304) then
		return false;
	end
	if not task:HasSubmitedTask(50203) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00050304()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(50304) or task:HasCompletedTask(50304) or task:HasSubmitedTask(50304) then
		return false;
	end
	if not task:HasSubmitedTask(50203) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00050304()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50304) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00050304(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50304) == npcId and Task_Accept_00050304 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50304
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ‹›å‹Ÿå¼ è§’";
	elseif task:HasAcceptedTask(50304) and npcId == 139 then
		RunAutoBattleAction(player, 139, -1);
	elseif task:GetTaskSubmitNpc(50304) == npcId and Task_Submit_00050304() then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50304
		action.m_ActionToken = 2;
		action.m_ActionStep = 10;
		action.m_ActionMsg = "æ‹›å‹Ÿå¼ è§’";
	end
	
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00050304_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "æ‹¥æœ‰äº†åˆºå®¢å’Œè°‹å£«ä¹‹åï¼Œä½ æœ‰æ²¡æœ‰æ˜æ˜¾æ„Ÿåˆ°å®åŠ›å¤§å¢å•Šï¼Ÿè¿™å°±æ˜¯é˜Ÿä¼çš„åŠ›é‡ï¼Œå…¶å®ä»¥ä½ ç°åœ¨çš„ç»Ÿé¢†èƒ½åŠ›è¿˜å¯ä»¥å†æ·»ä¸€å‘˜";
	action.m_ActionMsg = "æ˜¯å˜›ï¼Œè¯·ä½ æŒ‡ç‚¹æˆ‘ã€‚";
	return action;
end

function Task_00050304_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç°åœ¨çš„ä½ è¿˜ç¼ºå°‘ä¸€åçŒ›å°†ï¼Œå»è¯•ç€æŒ‘æˆ˜é•‡å‰æ‰€çš„å¼ è§’å§ã€‚";
	action.m_ActionMsg = "äº‹ä¸å®œè¿Ÿï¼Œæˆ‘ç°åœ¨å°±å»ã€‚";
	return action;
end

function Task_00050304_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "åœ¨ä¸‹æ„¿æ•ˆçŠ¬é©¬ä¹‹åŠ³ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050304_step_table = {
		[1] = Task_00050304_step_01,
		[2] = Task_00050304_step_02,
		[10] = Task_00050304_step_10,
		};

function Task_00050304_step(step)
	if Task_00050304_step_table[step] ~= nil then
		return Task_00050304_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00050304_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050304() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50304) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00050304_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	
	if player:isFighterFull() then
		player:sendMsgCode(0, 2006);
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(50304) then
		return false;
	end
	player:takeFighter(139, true);

	player:AddExp(3000);
	player:getCoin(10000);
	player:getTael(150);

	return true;
end

--·ÅÆúÈÎÎñ
function Task_00050304_abandon()
	return GetPlayer():GetTaskMgr():AbandonTask(50304);
end
