--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000022()
	local player = GetPlayer();
	if player:GetLev() < 11 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(22) or task:HasCompletedTask(22) or task:HasSubmitedTask(22) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(21) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(21) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(21) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000022()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 11 then
		return false;
	end
	if task:HasAcceptedTask(22) or task:HasCompletedTask(22) or task:HasSubmitedTask(22) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(21) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(21) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(21) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000022()
	if GetPlayer():GetTaskMgr():HasCompletedTask(22) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000022(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(22) == npcId and Task_Accept_00000022 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 22
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ€’æƒ©æ¯›å¤ª";
	elseif task:GetTaskSubmitNpc(22) == npcId then
		if Task_Submit_00000022() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 22
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ€’æƒ©æ¯›å¤ª";
		elseif task:HasAcceptedTask(22) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 22
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ€’æƒ©æ¯›å¤ª";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000022_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ æ•‘å‘½ï¼Œè¿™ä¸ªå’Œå°šè·Ÿè¸ªæˆ‘åˆ°å®¶é‡Œï¼Œè§å‘¨å›´æ— äººä¾¿æƒ³éç¤¼äºæˆ‘ï¼Œæˆ‘çœ‹å°‘ä¾ ä¸€èº«æ­£æ°”ï¼Œè‚¯å®šæ˜¯æ­£é“çš„å‰‘ä»™ï¼Œå¸Œæœ›å¯ä»¥å¸®æˆ‘åˆšèµ°è¿™ä¸ªèŠ±å’Œå°šã€‚";
	action.m_ActionMsg = "å¥½çš„ï¼Œçœ‹æˆ‘å¸®ä½ èµ¶è·‘è¿™ä¸ªæ¶äººã€‚";
	return action;
end

function Task_00000022_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¤šè°¢å°‘ä¾ å‡ºæ‰‹ç›¸åŠ©ï¼Œå°å¥³å­æ„Ÿæ¿€ä¸å°½ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000022_step_table = {
		[1] = Task_00000022_step_01,
		[10] = Task_00000022_step_10,
		};

function Task_00000022_step(step)
	if Task_00000022_step_table[step] ~= nil then
		return Task_00000022_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000022_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000022() then
		return false;
	end
	if not task:AcceptTask(22) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000022_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(22) then
		return false;
	end


	player:AddExp(3500);
	player:getTael(200);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000022_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(22);
end
