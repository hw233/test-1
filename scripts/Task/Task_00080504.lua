--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080504()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80504) or task:HasCompletedTask(80504) or task:HasSubmitedTask(80504) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80503) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80503) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00080504()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80504) or task:HasCompletedTask(80504) or task:HasSubmitedTask(80504) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80503) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80503) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00080504()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80504) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080504(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80504) == npcId and Task_Accept_00080504 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80504
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ·æŠ¥";
	elseif task:GetTaskSubmitNpc(80504) == npcId then
		if Task_Submit_00080504() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80504
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ·æŠ¥";
		elseif task:HasAcceptedTask(80504) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80504
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ·æŠ¥";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080504_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "è¿™å°±æ˜¯æ€é¸¡å„†çŒ´ï¼Œçœ‹é‚£äº›å†°å¦–å’Œå…¶ä»–å°æ€ªè¿˜æ•¢ä¸æ•¢å‹¾ç»“å¤œæ‘©ç›Ÿã€‚";
	action.m_ActionMsg = "æƒ³ä¸åˆ°å¤œæ‘©ç›Ÿçš„çˆªç‰™å·²éå¸ƒå„å¤„ã€‚";
	return action;
end

function Task_00080504_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "éƒ½æ˜¯äº›è™¾å…µèŸ¹å°†ä¸è¶³ä¸ºæ‚£ï¼Œä½†æˆ‘ä»¬ä¹Ÿä¸èƒ½å¤ªè¿‡äºè½»æ•Œã€‚";
	action.m_ActionMsg = "æˆ‘ä»¬ç°åœ¨å¾—æ­¥æ­¥å°å¿ƒäº†ã€‚";
	return action;
end

function Task_00080504_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å½¢åŠ¿ä¸å®¹ä¹è§‚ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080504_step_table = {
		[1] = Task_00080504_step_01,
		[2] = Task_00080504_step_02,
		[10] = Task_00080504_step_10,
		};

function Task_00080504_step(step)
	if Task_00080504_step_table[step] ~= nil then
		return Task_00080504_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080504_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080504() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80504) then
		return false;
	end
	task:AddTaskStep(80504);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00080504_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80504) then
		return false;
	end


	player:AddExp(525000);
	player:getCoin(302000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080504_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80504);
end
