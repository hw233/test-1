--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00040106()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 10 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40106) or task:HasCompletedTask(40106) or task:HasSubmitedTask(40106) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00040106()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 10 then
		return false;
	end
	if task:HasAcceptedTask(40106) or task:HasCompletedTask(40106) or task:HasSubmitedTask(40106) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00040106()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40106) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00040106(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40106) == npcId and Task_Accept_00040106 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40106
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ‹›å‹Ÿè²‚è‰";
	elseif task:HasAcceptedTask(40106) and npcId == 134 then
		RunAutoBattleAction(player, 134, -1);
	elseif task:GetTaskSubmitNpc(40106) == npcId and Task_Submit_00040106() then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40106
		action.m_ActionToken = 2;
		action.m_ActionStep = 10;
		action.m_ActionMsg = "æ‹›å‹Ÿè²‚è‰";	
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00040106_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä¿®ç‚¼ä¹‹è·¯å°†ä¼šé‡åˆ°å„ç§è‰°éš¾é™©é˜»ï¼Œåªå‡­ä¸€å·±ä¹‹åŠ›æ˜¯ä¸å¤Ÿçš„ï¼Œå¤©æ—æœ‰å¾ˆå¤šéšå±…çš„åå°†ï¼Œè²‚è‰æ˜¯å…¶ä¸­ä¸€ä¸ªã€‚ä½ éœ€è¦å»æ‰“è´¥å¹¶æ”¶æœå¥¹ï¼Œè®©å¥¹æˆä¸ºä½ çš„å£«å…µï¼ŒåŒä½ å¹¶è‚©ä½œæˆ˜ã€‚";
	action.m_ActionMsg = "è¿™æœ‰ä»€ä¹ˆéš¾çš„ï¼Œæˆ‘ä¼šè®©å¥¹è¯šæœäºæˆ‘çš„ã€‚";
	return action;
end

function Task_00040106_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¦¾èº«æ„¿è·Ÿéšä½ å¾æˆ˜åˆ°å¤©æ¶¯æµ·è§’ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040106_step_table = {
		[1] = Task_00040106_step_01,
		[10] = Task_00040106_step_10,
		};

function Task_00040106_step(step)
	if Task_00040106_step_table[step] ~= nil then
		return Task_00040106_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00040106_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040106() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40106) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00040106_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if player:isFighterFull() then
		player:sendMsgCode(0, 2006);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(40106) then
		return false;
	end


	player:AddExp(1000);
	player:getCoin(10000);
	player:getTael(60);
	player:takeFighter(134, true);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00040106_abandon()
	return GetPlayer():GetTaskMgr():AbandonTask(40106);
end
