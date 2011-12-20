--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000189()
	local player = GetPlayer();
	if player:GetLev() < 90 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(189) or task:HasCompletedTask(189) or task:HasSubmitedTask(189) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(188) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(188) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(188) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000189()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 90 then
		return false;
	end
	if task:HasAcceptedTask(189) or task:HasCompletedTask(189) or task:HasSubmitedTask(189) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(188) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(188) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(188) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000189()
	if GetPlayer():GetTaskMgr():HasCompletedTask(189) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000189(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(189) == npcId and Task_Accept_00000189 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 189
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç™½æ³½";
	elseif task:GetTaskSubmitNpc(189) == npcId then
		if Task_Submit_00000189() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 189
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç™½æ³½";
		elseif task:HasAcceptedTask(189) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 189
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç™½æ³½";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000189_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è€å¤«ä¹…å±…æ‘©äº‘å²­ï¼Œé™„è¿‘æœ‰ä¸€å¤„ç¾æ™¯å«æ˜Ÿå®¿æµ·ï¼Œå¯æ˜¯ä¸æ—¥å‰æ¥äº†ä¸€åªå¼‚å…½ç™½æ³½ï¼Œç ´åæ¤è¢«ï¼Œæ€æ­»å…¶ä»–ç”Ÿçµæ— æ•°ã€‚è¿œè¿œè§åˆ°è€å¤«çš„æ°”æ¯å°±é€ƒéçš„æ— å½±æ— è¸ªï¼Œæ‰€ä»¥ç‰¹åœ°éº»çƒ¦å°‘ä¾ æ¥å¸®æˆ‘è§£å†³è¿™ä¸ªå¼‚å…½äº†ã€‚";
	action.m_ActionMsg = "æˆ‘è¿™å°±å»å°†è¿™åªç™½æ³½æŠ“æ¥ã€‚";
	return action;
end

function Task_00000189_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ çœŸæ˜¯ä¿®ä¸ºæ·±åšï¼Œå¤©èµ‹å¼‚ç¦€å•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000189_step_table = {
		[1] = Task_00000189_step_01,
		[10] = Task_00000189_step_10,
		};

function Task_00000189_step(step)
	if Task_00000189_step_table[step] ~= nil then
		return Task_00000189_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000189_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000189() then
		return false;
	end
	if not task:AcceptTask(189) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000189_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(189) then
		return false;
	end


	player:AddExp(150000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000189_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(189);
end
