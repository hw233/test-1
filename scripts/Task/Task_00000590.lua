--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000590()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(590) or task:HasCompletedTask(590) or task:HasSubmitedTask(590) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000590()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(590) or task:HasCompletedTask(590) or task:HasSubmitedTask(590) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000590()
	if GetPlayer():GetTaskMgr():HasCompletedTask(590) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000590(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(590) == npcId and Task_Accept_00000590 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 590
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¯’æ³‰æ¶äºº";
	elseif task:GetTaskSubmitNpc(590) == npcId then
		if Task_Submit_00000590() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 590
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¯’æ³‰æ¶äºº";
		elseif task:HasAcceptedTask(590) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 590
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¯’æ³‰æ¶äºº";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000590_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆéƒ½ä¸è¿œå¤„çš„æ¡‚èŠ±å±±æ˜¯ä¸€å¤„ç¾æ™¯ï¼Œä¸è¿‡å±±ä¸­æœ‰äº”è‰²äº‘é›¾ï¼Œå¯»å¸¸äººè½»æ˜“ä¸å¾—å…¥å†…ï¼Œå‰ä¸ä¹…é£é¾™å¸ˆå¤ªçš„å¾’å¼Ÿé‡‘é©¼åœ¨æ¡‚èŠ±å±±æ€æ­»äº†æˆ‘æ´¾å¼Ÿå­æ•°äººï¼Œè¿™ç§é‚ªæ¶ä¹‹å¾’äººäººå¾—è€Œè¯›ä¹‹ï¼Œä½ å»è·‘ä¸€è¶Ÿå§ã€‚";
	action.m_ActionMsg = "å¼Ÿå­éµå‘½ï¼Œæˆ‘å»å»å°±å›ã€‚";
	return action;
end

function Task_00000590_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ çœŸæ˜¯èº«æ‰‹ä¸å‡¡å•Šã€‚ ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000590_step_table = {
		[1] = Task_00000590_step_01,
		[10] = Task_00000590_step_10,
		};

function Task_00000590_step(step)
	if Task_00000590_step_table[step] ~= nil then
		return Task_00000590_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000590_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000590() then
		return false;
	end
	if not task:AcceptTask(590) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000590_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(590) then
		return false;
	end


	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000590_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(590);
end
