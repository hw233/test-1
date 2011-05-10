--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090107()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90107) or task:HasCompletedTask(90107) or task:HasSubmitedTask(90107) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90106) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90106) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090107()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90107) or task:HasCompletedTask(90107) or task:HasSubmitedTask(90107) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90106) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90106) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090107()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90107) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090107(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90107) == npcId and Task_Accept_00090107 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90107
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¯æ€•çš„æ¢¦å¢ƒ";
	elseif task:GetTaskSubmitNpc(90107) == npcId then
		if Task_Submit_00090107() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90107
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¯æ€•çš„æ¢¦å¢ƒ";
		elseif task:HasAcceptedTask(90107) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90107
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¯æ€•çš„æ¢¦å¢ƒ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090107_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "ä½ æ˜¯è°ï¼Ÿæˆ‘è®¤è¯†ä½ å—ï¼Ÿ";
	action.m_ActionMsg = "æˆ‘æ˜¯ä»£å·«å¸ˆæ¥çœ‹ä½ çš„ï¼Œæœ€è¿‘å¯å¥½ï¼Ÿ";
	return action;
end

function Task_00090107_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 3;
	action.m_NpcMsg = "å“¦ï¼Œè°¢è°¢ä½ ã€‚æˆ‘ä¸å¤ªå¥½ï¼Œæœ€è¿‘æ€»åšå™©æ¢¦ï¼Œæ¢¦è§æµ·åº•æœ‰ä¸€ç¾¤æµè¡€çš„éª·é«…ï¼Œå¤ªææ€–äº†ã€‚";
	action.m_ActionMsg = "æµè¡€çš„éª·é«…ï¼Ÿéš¾å€’æ˜¯æ²™æ³¢æµ·åº•å¤çªŸçš„æ®‹è¡€éª·é«…åœ¨ä½œæ€ªï¼Ÿï¼ˆæ²‰æ€ä¸­ï¼‰";
	return action;
end

function Task_00090107_step_03()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æµ·åº•ï¼Ÿä½ èƒ½å¸®æˆ‘å»çœ‹çœ‹å—ï¼Ÿ";
	action.m_ActionMsg = "å°äº‹ä¸€æ¡©ï¼Œä½ ç­‰æˆ‘ã€‚";
	return action;
end

function Task_00090107_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "çœŸçš„æ˜¯é‚£äº›æ€ªç‰©åœ¨å®³æˆ‘å—ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090107_step_table = {
		[1] = Task_00090107_step_01,
		[2] = Task_00090107_step_02,
		[3] = Task_00090107_step_03,
		[10] = Task_00090107_step_10,
		};

function Task_00090107_step(step)
	if Task_00090107_step_table[step] ~= nil then
		return Task_00090107_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090107_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090107() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90107) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090107_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90107) then
		return false;
	end


	player:AddExp(575000);
	player:getCoin(306000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090107_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90107);
end
