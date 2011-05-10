--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080503()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80503) or task:HasCompletedTask(80503) or task:HasSubmitedTask(80503) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80502) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80502) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00080503()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80503) or task:HasCompletedTask(80503) or task:HasSubmitedTask(80503) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80502) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80502) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00080503()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80503) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080503(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80503) == npcId and Task_Accept_00080503 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80503
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å‰¿ç­é»‘é£å¯¨";
	elseif task:GetTaskSubmitNpc(80503) == npcId then
		if Task_Submit_00080503() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80503
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å‰¿ç­é»‘é£å¯¨";
		elseif task:HasAcceptedTask(80503) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80503
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å‰¿ç­é»‘é£å¯¨";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080503_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "é»‘é£å¯¨æƒ…å†µå¦‚ä½•ï¼Ÿ";
	action.m_ActionMsg = "ä»–ä»¬æœç„¶ä¸å¤œæ‘©ç›Ÿå‹¾ç»“ï¼Œè€Œä¸”åŠ¿åŠ›è¶Šæ¥è¶Šå¤§ï¼Œæ®è¯´è¿å†°å¦–éƒ½å‚ä¸äº†ä»–ä»¬çš„è®¡åˆ’ã€‚";
	return action;
end

function Task_00080503_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æœçœŸå¦‚æ­¤ï¼Ÿé‚£ä¸èƒ½å†æ”¾çºµä»–ä»¬ä»»æ„è€Œä¸ºï¼Œä½ å»å‰¿æ€ä»–ä»¬çš„å·¢ç©´ã€‚";
	action.m_ActionMsg = "ä½†æ„¿ä¸ä¼šå¼•æ¥æ›´å¤§çš„å±æœºã€‚";
	return action;
end

function Task_00080503_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™å°±æ˜¯æ€é¸¡å„†çŒ´ï¼Œçœ‹é‚£äº›å†°å¦–å’Œå…¶ä»–å°æ€ªè¿˜æ•¢ä¸æ•¢å‹¾ç»“å¤œæ‘©ç›Ÿã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080503_step_table = {
		[1] = Task_00080503_step_01,
		[2] = Task_00080503_step_02,
		[10] = Task_00080503_step_10,
		};

function Task_00080503_step(step)
	if Task_00080503_step_table[step] ~= nil then
		return Task_00080503_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080503_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080503() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80503) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00080503_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80503) then
		return false;
	end


	player:AddExp(525000);
	player:getCoin(302000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080503_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80503);
end
