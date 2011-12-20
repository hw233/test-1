--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000178()
	local player = GetPlayer();
	if player:GetLev() < 85 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(178) or task:HasCompletedTask(178) or task:HasSubmitedTask(178) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000178()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 85 then
		return false;
	end
	if task:HasAcceptedTask(178) or task:HasCompletedTask(178) or task:HasSubmitedTask(178) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000178()
	if GetPlayer():GetTaskMgr():HasCompletedTask(178) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000178(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(178) == npcId and Task_Accept_00000178 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 178
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å­½é¾™æ½­";
	elseif task:GetTaskSubmitNpc(178) == npcId then
		if Task_Submit_00000178() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 178
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å­½é¾™æ½­";
		elseif task:HasAcceptedTask(178) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 178
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å­½é¾™æ½­";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000178_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è®°å¾—æ•°æœˆå‰ï¼Œå°‘ä¾ å¸®æˆ‘åœ¨æ–©é¾™å´–é™¤æ‰çš„å°è›‡å—ï¼Œå¦‚ä»Šå´æƒ¹å‡ºäº†é‚£åªå°è›‡çš„é•¿è¾ˆå‡ºæ¥ï¼Œåœ¨å­½é¾™æ½­å…´é£ä½œæµªã€‚å¯æ˜¯æˆ‘ä»¬æ­¦å½“æ‰é­å¤§å˜ï¼Œæˆ‘è…¾ä¸å‡ºæ‰‹æ¥æ”¶æ‹¾ä»–ï¼Œè¿˜éº»çƒ¦å°‘ä¾ å¸®å¿™ã€‚";
	action.m_ActionMsg = "æ˜¯å—ï¼Œæˆ‘è¿™å°±å»çœ‹çœ‹ã€‚";
	return action;
end

function Task_00000178_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ çœŸæ˜¯ä¿®ä¸ºæ·±åšï¼Œå¤©èµ‹å¼‚ç¦€å•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000178_step_table = {
		[1] = Task_00000178_step_01,
		[10] = Task_00000178_step_10,
		};

function Task_00000178_step(step)
	if Task_00000178_step_table[step] ~= nil then
		return Task_00000178_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000178_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000178() then
		return false;
	end
	if not task:AcceptTask(178) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000178_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(178) then
		return false;
	end


	player:AddExp(150000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000178_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(178);
end
