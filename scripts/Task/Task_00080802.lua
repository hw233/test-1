--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080802()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80802) or task:HasCompletedTask(80802) or task:HasSubmitedTask(80802) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80801) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80801) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00080802()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80802) or task:HasCompletedTask(80802) or task:HasSubmitedTask(80802) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80801) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80801) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00080802()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80802) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080802(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80802) == npcId and Task_Accept_00080802 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80802
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "èƒœåˆ©çš„æ¶ˆæ¯";
	elseif task:GetTaskSubmitNpc(80802) == npcId then
		if Task_Submit_00080802() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80802
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "èƒœåˆ©çš„æ¶ˆæ¯";
		elseif task:HasAcceptedTask(80802) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80802
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "èƒœåˆ©çš„æ¶ˆæ¯";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080802_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ å»å‘ç‹ä»å®«çš„å¯’å°§æ±‡æŠ¥æ­¤å¤„æˆ˜å†µï¼Œè®©ä»–æ”¾å¿ƒã€‚";
	action.m_ActionMsg = "æˆ‘å®šä¼šå¦‚å®ç¦€æŠ¥ã€‚";
	return action;
end

function Task_00080802_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘ä¸€ç›´åœ¨è¿™ç­‰ä½ ï¼Œæˆ‘çš„å‹‡å£«ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080802_step_table = {
		[1] = Task_00080802_step_01,
		[10] = Task_00080802_step_10,
		};

function Task_00080802_step(step)
	if Task_00080802_step_table[step] ~= nil then
		return Task_00080802_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080802_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080802() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80802) then
		return false;
	end
	task:AddTaskStep(80802);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00080802_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80802) then
		return false;
	end


	player:AddExp(725000);
	player:getCoin(318000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080802_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80802);
end
