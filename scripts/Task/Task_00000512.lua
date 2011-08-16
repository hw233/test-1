--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000512()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(512) or task:HasCompletedTask(512) or task:HasSubmitedTask(512) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000512()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(512) or task:HasCompletedTask(512) or task:HasSubmitedTask(512) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000512()
	if GetPlayer():GetTaskMgr():HasCompletedTask(512) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000512(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(512) == npcId and Task_Accept_00000512 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 512
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "éšå±…å‰‘ä¾ ";
	elseif task:GetTaskSubmitNpc(512) == npcId then
		if Task_Submit_00000512() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 512
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "éšå±…å‰‘ä¾ ";
		elseif task:HasAcceptedTask(512) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 512
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "éšå±…å‰‘ä¾ ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000512_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "åœ¨æˆéƒ½æœ‰ä½éšå±…å¤šå¹´çš„å‰‘ä¾ ï¼Œå«æ²ˆæµªï¼Œæ˜”å¹´ä¸ºå¸ˆæ›¾å’Œä»–ä¸€èµ·å‡ºé—¨å†ç»ƒï¼Œé™é­”é™¤å¦–æ— æ•°ï¼Œåªæ˜¯æ±‰å®¤é¢ è¦†ï¼Œåˆæªäºä¿®é“ä¹‹äººçš„æˆ’å¾‹ï¼Œä¸èƒ½å¯¹å‡¡äººå‡ºæ‰‹ï¼Œæ‰€ä»¥ä»–å¿ƒç°æ„å†·éšå±…æˆéƒ½ï¼Œä½ å»æ‹œè®¿ä¸€ä¸‹ä»–å§ã€‚";
	action.m_ActionMsg = "å¼Ÿå­è¿™å°±å»ã€‚";
	return action;
end

function Task_00000512_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ•°åå¹´æ²¡è§äº†ï¼Œæƒ³ä¸åˆ°é½æŒæ•™è¿˜è®°å¾—æˆ‘ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000512_step_table = {
		[1] = Task_00000512_step_01,
		[10] = Task_00000512_step_10,
		};

function Task_00000512_step(step)
	if Task_00000512_step_table[step] ~= nil then
		return Task_00000512_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000512_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000512() then
		return false;
	end
	if not task:AcceptTask(512) then
		return false;
	end
	task:AddTaskStep(512);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000512_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(512) then
		return false;
	end


	player:AddExp(8000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000512_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(512);
end
