--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090105()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90105) or task:HasCompletedTask(90105) or task:HasSubmitedTask(90105) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90104) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90104) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090105()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90105) or task:HasCompletedTask(90105) or task:HasSubmitedTask(90105) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90104) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90104) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090105()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90105) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090105(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90105) == npcId and Task_Accept_00090105 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90105
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "è¯•è¯";
	elseif task:GetTaskSubmitNpc(90105) == npcId then
		if Task_Submit_00090105() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90105
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "è¯•è¯";
		elseif task:HasAcceptedTask(90105) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90105
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "è¯•è¯";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090105_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‹‡å£«ï¼Œä½ ç°åœ¨åƒä¸‹è¿™ç¥è¯ï¼Œç„¶åå»æŒ‘æˆ˜ç½—æ‘©ï¼Œçœ‹çœ‹ç¥è¯æ˜¯å¦èƒ½å¢å¼ºä½ çš„æˆ˜æ–—åŠ›ã€‚";
	action.m_ActionMsg = "è¿™ä¹ˆç¥å¥‡ï¼Ÿï¼ˆåƒä¸‹ï¼‰æˆ‘å»è¯•è¯•ï¼Œç½—æ‘©å¯æ˜¯ç¥å‹‡æ— æ¯”çš„å°†å†›ã€‚";
	return action;
end

function Task_00090105_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘æ˜¯ä¸–ä¸Šæœ€å¼ºçš„é­”æ³•å·«å¸ˆã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090105_step_table = {
		[1] = Task_00090105_step_01,
		[10] = Task_00090105_step_10,
		};

function Task_00090105_step(step)
	if Task_00090105_step_table[step] ~= nil then
		return Task_00090105_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090105_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090105() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90105) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090105_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90105) then
		return false;
	end


	player:AddExp(550000);
	player:getCoin(304000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090105_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90105);
end
