--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000655()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(655) or task:HasCompletedTask(655) or task:HasSubmitedTask(655) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000655()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(655) or task:HasCompletedTask(655) or task:HasSubmitedTask(655) or not player:isClanTask(655) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000655()
	if GetPlayer():GetTaskMgr():HasCompletedTask(655) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000655(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(655) == npcId and Task_Accept_00000655 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 655
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "é‡‡èŠ±å¤§ç›—";
	elseif task:GetTaskSubmitNpc(655) == npcId then
		if Task_Submit_00000655() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 655
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "é‡‡èŠ±å¤§ç›—";
		elseif task:HasAcceptedTask(655) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 655
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "é‡‡èŠ±å¤§ç›—";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000655_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆéƒ½å‰æ®µæ—¶é—´æ²»å®‰çœŸå¾—å¾ˆæˆé—®é¢˜ï¼Œæœ‰å‡ ä¸ªå¤§æˆ·äººå®¶çš„å°å§éƒ½è¢«é‚£é‡‡èŠ±å¤§ç›—ç³Ÿè¹‹ï¼Œè¿˜å¸Œæœ›"..GetPlayerName(GetPlayer()).."å»å°†è¿™ä¸ªæ·«è´¼æ‰æ‹¿å½’æ¡ˆã€‚";
	action.m_ActionMsg = "ä¸ºæ°‘é™¤å®³æ˜¯æˆ‘è¾ˆæœ¬è‰²ï¼Œå°å¯å»å»å°±å›ã€‚";
	return action;
end

function Task_00000655_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ çœŸæ˜¯èº«æ‰‹ä¸å‡¡å•Šã€‚ ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000655_step_table = {
		[1] = Task_00000655_step_01,
		[10] = Task_00000655_step_10,
		};

function Task_00000655_step(step)
	if Task_00000655_step_table[step] ~= nil then
		return Task_00000655_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000655_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000655() then
		return false;
	end
	if not task:AcceptTask(655) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000655_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(655) then
		return false;
	end


	player:AddExp(5000);
	player:AddClanBuilding(200);
	player:AddClanContrib(200);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000655_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(655);
end
