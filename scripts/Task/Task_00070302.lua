--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00070302()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(70302) or task:HasCompletedTask(70302) or task:HasSubmitedTask(70302) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00070302()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(70302) or task:HasCompletedTask(70302) or task:HasSubmitedTask(70302) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	if task:GetDayTaskCurrTaskId(1003) ~= 70302 then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00070302()
	if GetPlayer():GetTaskMgr():HasCompletedTask(70302) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00070302(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(70302) == npcId and Task_Accept_00070302 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 70302
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ä¸€ä¸ªéƒ½ä¸èƒ½ç•™";
	elseif task:GetTaskSubmitNpc(70302) == npcId then
		if Task_Submit_00070302() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70302
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ä¸€ä¸ªéƒ½ä¸èƒ½ç•™";
		elseif task:HasAcceptedTask(70302) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70302
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ä¸€ä¸ªéƒ½ä¸èƒ½ç•™";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00070302_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¥¿æ¥å±±è°·çš„åƒµå°¸å°„æ‰‹åˆ¶é€ çš„éº»çƒ¦è¶Šæ¥è¶Šå¤šäº†ï¼Œä¸ä»…æ˜¯æˆ‘ï¼Œè®¸å¤šå‘¨å›´çš„äººä¹Ÿä¸æ–­å—åˆ°éº»çƒ¦ï¼Œæˆ‘éœ€è¦ä½ æŒºèº«è€Œå‡ºæ¥ï¼Œå»è§‚éŸ³å±±çš„è¥¿æ¥å±±è°·è§£å†³ä»–ä»¬ï¼Œæ— è®ºå¦‚ä½•ä¸€ä¸ªéƒ½ä¸èƒ½ç•™ã€‚";
	action.m_ActionMsg = "æˆ‘å®šèƒ½å°†ä»–ä»¬æ€ä¸ªç‰‡ç”²ä¸ç•™ï¼Œä½ æ”¾å¿ƒï¼";
	return action;
end

function Task_00070302_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ çœŸçš„æŠŠé‚£äº›åƒµå°¸å°„æ‰‹æ‰“çš„ä¸€ä¸ªéƒ½æ²¡æœ‰ç•™ä¸‹ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00070302_step_table = {
		[1] = Task_00070302_step_01,
		[10] = Task_00070302_step_10,
		};

function Task_00070302_step(step)
	if Task_00070302_step_table[step] ~= nil then
		return Task_00070302_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00070302_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00070302() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(70302) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00070302_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(70302) then
		if DayTaskAward(1003) then
			task:DayTaskSubmit(70302);
			return true;
		end
	end

	return false;
end

--·ÅÆúÈÎÎñ
function Task_00070302_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(70302);
end
