--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000059()
	local player = GetPlayer();
	if player:GetLev() < 35 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(59) or task:HasCompletedTask(59) or task:HasSubmitedTask(59) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(58) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(58) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(58) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000059()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 35 then
		return false;
	end
	if task:HasAcceptedTask(59) or task:HasCompletedTask(59) or task:HasSubmitedTask(59) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(58) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(58) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(58) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000059()
	if GetPlayer():GetTaskMgr():HasCompletedTask(59) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000059(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(59) == npcId and Task_Accept_00000059 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 59
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ä¹Œé£é…’";
	elseif task:GetTaskSubmitNpc(59) == npcId then
		if Task_Submit_00000059() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 59
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ä¹Œé£é…’";
		elseif task:HasAcceptedTask(59) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 59
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ä¹Œé£é…’";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000059_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç¦ä»™æ½­åº•çš„æ¯’çŸ³ç”Ÿé•¿äºåœ°ç«ä¹‹ä¸Šï¼Œä½ ä»¬æ–¹æ‰é‡‡é›†è‰è¯ä¹‹æ—¶å·²ç»æ²¾æŸ“äº†ç«æ¯’ï¼Œææ€•å¾ˆå¿«å°±ä¼šå‘ä½œã€‚å°å¾’é‚£é‡Œæœ‰æˆ‘å¤šå¹´å‰ç§˜åˆ¶çš„ä¹Œé£é…’å¯è§£æ­¤ç«æ¯’ï¼Œä½ ä»¬èµ¶å¿«å»å§ï¼Œæˆ‘ä¸ä¹…å°±å°†åœ†å¯‚é£å‡ï¼Œå°å¾’ä¹Ÿæ‹œæ‰˜ä½ ä»¬ç…§é¡¾äº†ã€‚";
	action.m_ActionMsg = "å§¥å§¥ä¿é‡ï¼Œæˆ‘ä»¬è¿™å°±èµ°äº†ã€‚";
	return action;
end

function Task_00000059_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¸ˆå‚…ä»å°æŠŠæˆ‘æŠšå…»é•¿å¤§ï¼Œå¦‚ä»Šç¦»å»ï¼Œè‹¥å…°æ„Ÿåˆ°ä¸èƒœæ‚²ä¼¤ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000059_step_table = {
		[1] = Task_00000059_step_01,
		[10] = Task_00000059_step_10,
		};

function Task_00000059_step(step)
	if Task_00000059_step_table[step] ~= nil then
		return Task_00000059_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000059_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000059() then
		return false;
	end
	if not task:AcceptTask(59) then
		return false;
	end
	task:AddTaskStep(59);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000059_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(59) then
		return false;
	end


	player:AddExp(10000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000059_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(59);
end
