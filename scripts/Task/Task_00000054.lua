--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000054()
	local player = GetPlayer();
	if player:GetLev() < 35 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(54) or task:HasCompletedTask(54) or task:HasSubmitedTask(54) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000054()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 35 then
		return false;
	end
	if task:HasAcceptedTask(54) or task:HasCompletedTask(54) or task:HasSubmitedTask(54) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000054()
	if GetPlayer():GetTaskMgr():HasCompletedTask(54) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000054(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(54) == npcId and Task_Accept_00000054 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 54
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æƒ©æ²»æ¶å¾’";
	elseif task:GetTaskSubmitNpc(54) == npcId then
		if Task_Submit_00000054() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 54
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æƒ©æ²»æ¶å¾’";
		elseif task:HasAcceptedTask(54) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 54
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æƒ©æ²»æ¶å¾’";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000054_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°å¦¹å·²åœ¨æ¡‚èŠ±å±±ç­‰å€™ä½ å¤šæ—¶äº†ï¼Œè¿™æ®µæ—¶é—´ä¸€ç›´è¢«é‚£é£é¾™å¸ˆå¤ªçš„å¼Ÿå­é‡‘é©¼éªšæ‰°ï¼Œæƒ³è¦æˆ‘å«ç»™ä»–ï¼Œç›®å‰é‡‘é©¼å’Œä»–ä¸¤ä¸ªå¸ˆå¦¹æ­£åœ¨å¯’æ³‰ï¼Œå¸Œæœ›å°‘ä¾ å¯ä»¥å»å¸®æˆ‘æ•™è®­ä»–ä»¬ä¸€ä¸‹ã€‚";
	action.m_ActionMsg = "å°äº‹ä¸€ä»¶ï¼Œå°±åŒ…åœ¨æˆ‘èº«ä¸Šäº†ã€‚";
	return action;
end

function Task_00000054_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000054_step_table = {
		[1] = Task_00000054_step_01,
		[10] = Task_00000054_step_10,
		};

function Task_00000054_step(step)
	if Task_00000054_step_table[step] ~= nil then
		return Task_00000054_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000054_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000054() then
		return false;
	end
	if not task:AcceptTask(54) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000054_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(54) then
		return false;
	end


	player:AddExp(3333);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000054_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(54);
end
