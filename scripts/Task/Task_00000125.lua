--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000125()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(125) or task:HasCompletedTask(125) or task:HasSubmitedTask(125) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(48) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(48) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(48) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000125()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(125) or task:HasCompletedTask(125) or task:HasSubmitedTask(125) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(48) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(48) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(48) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000125()
	if GetPlayer():GetTaskMgr():HasCompletedTask(125) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000125(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(125) == npcId and Task_Accept_00000125 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 125
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ£˜æ‰‹æ¡ˆä»¶";
	elseif task:GetTaskSubmitNpc(125) == npcId then
		if Task_Submit_00000125() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 125
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ£˜æ‰‹æ¡ˆä»¶";
		elseif task:HasAcceptedTask(125) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 125
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ£˜æ‰‹æ¡ˆä»¶";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000125_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ï¼Œå¦‚ä»Šä¸­åå¤§åœ°å…µæ‚£æ‰æ¯ï¼Œé‚ªé“ä¹‹äººçº·ä¹±åˆèµ·ï¼Œæˆ‘ç­‰æ­£é“äººå£«æ­£åº”æ¶ˆç­å¦–é‚ªï¼Œç»´æŠ¤æ­£ä¹‰ã€‚å¬è¯´æˆéƒ½åºœè¡™èµµæ•å¤´é‡åˆ°äº†å‡ ä»¶æ£˜æ‰‹çš„æ¡ˆå­ï¼Œä½ å»å¸®å¸®ä»–å§ã€‚ ";
	action.m_ActionMsg = "";
	return action;
end

function Task_00000125_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¥½çš„ï¼Œæˆ‘è¿™å°±å»æ‰¾æ•å¤´å¤§å“¥ã€‚";
	action.m_ActionMsg = "";
	return action;
end

function Task_00000125_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ï¼Œä½ èƒ½æ¥å¸®æˆ‘çœŸæ˜¯å¤ªå¥½äº†ï¼Œæˆ‘ä»¬å‡¡ä¿—ä¹‹äººè§£å†³ä¸äº†é‚£äº›ç¥å¼‚ä¹‹äº‹å•Šï¼";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000125_step_table = {
		[1] = Task_00000125_step_01,
		[2] = Task_00000125_step_02,
		[10] = Task_00000125_step_10,
		};

function Task_00000125_step(step)
	if Task_00000125_step_table[step] ~= nil then
		return Task_00000125_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000125_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000125() then
		return false;
	end
	if not task:AcceptTask(125) then
		return false;
	end
	task:AddTaskStep(125);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000125_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(125) then
		return false;
	end


	player:AddExp(2000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000125_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(125);
end
