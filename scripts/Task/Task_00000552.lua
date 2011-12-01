--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000552()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(552) or task:HasCompletedTask(552) or task:HasSubmitedTask(552) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000552()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(552) or task:HasCompletedTask(552) or task:HasSubmitedTask(552) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000552()
	if GetPlayer():GetTaskMgr():HasCompletedTask(552) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000552(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(552) == npcId and Task_Accept_00000552 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 552
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "è›‡å¦–ä¹‹ç¥¸";
	elseif task:GetTaskSubmitNpc(552) == npcId then
		if Task_Submit_00000552() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 552
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "è›‡å¦–ä¹‹ç¥¸";
		elseif task:HasAcceptedTask(552) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 552
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "è›‡å¦–ä¹‹ç¥¸";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000552_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆéƒ½éƒŠå¤–çš„äº‘çµå±±æœ‰ä¸€åªè›‡å¦–ç›˜è¸ï¼Œç»å¸¸ç¥¸å®³è¿‡å¾€è¡Œäººã€‚è¿™åªè›‡å¦–æƒ¯äºå–·æ¶‚å‰§æ¯’ï¼Œæˆéƒ½çŸ¥åºœåŠå…¶æ‰‹ä¸‹çš„è¡™å½¹æ•å¿«æ‹¿å®ƒæ¯«æ— åŠæ³•ï¼Œæ‰€ä»¥ä¸Šæˆ‘ä»¬æ˜†ä»‘æ±‚åŠ©ï¼Œ"..GetPlayerName(GetPlayer()).."ä½ å»å°†è¿™åªå¦–ç‰©æ¶ˆç­å§ã€‚";
	action.m_ActionMsg = "ä¸ºæ°‘é™¤å®³æ˜¯æˆ‘è¾ˆæœ¬è‰²ï¼Œå¼Ÿå­å»å»å°±å›ã€‚";
	return action;
end

function Task_00000552_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é™¤äº†æ­¤è›‡å¦–ï¼Œä¹Ÿå¯ä»¥ä¿æˆéƒ½é™„è¿‘ç™¾å§“çš„å®‰å®ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000552_step_table = {
		[1] = Task_00000552_step_01,
		[10] = Task_00000552_step_10,
		};

function Task_00000552_step(step)
	if Task_00000552_step_table[step] ~= nil then
		return Task_00000552_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000552_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000552() then
		return false;
	end
	if not task:AcceptTask(552) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000552_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(552) then
		return false;
	end


	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000552_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(552);
end
