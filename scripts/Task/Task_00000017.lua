--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000017()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(17) or task:HasCompletedTask(17) or task:HasSubmitedTask(17) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(16) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(16) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000017()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(17) or task:HasCompletedTask(17) or task:HasSubmitedTask(17) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(16) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(16) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000017()
	if GetPlayer():GetTaskMgr():HasCompletedTask(17) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000017(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(17) == npcId and Task_Accept_00000017 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 17
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ•‘å‘½çµè¯";
	elseif task:GetTaskSubmitNpc(17) == npcId then
		if Task_Submit_00000017() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 17
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ•‘å‘½çµè¯";
		elseif task:HasAcceptedTask(17) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 17
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ•‘å‘½çµè¯";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000017_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™é™„è¿‘æœ‰ä¸€åº§äº‘çµå±±ï¼Œå±±ä¸­æœ‰ä¸€åªè›‡å¦–ä¸ºç¥¸ï¼Œæˆ‘å’Œå¤§å¸ˆæ­£ç€æ‰‹å‡†å¤‡é™¤æ‰è›‡å’¬ï¼Œè„±ä¸å¼€èº«ã€‚æˆ‘é‚£å¾’å„¿æ–½æ—æ–¹æ‰æ¢æŸ¥è›‡å¦–æ´ç©´è¯¯ä¸­æ¯’é›¾ï¼Œæ­£å¥½å°‘ä¾ ä½ å¯ä»¥ä¸ºæˆ‘é€è§£æ¯’è¯ç»™ä»–ï¼Œå°±åœ¨éƒŠå¤–çš„å†œå®¶èŒ…å±‹å¤„ã€‚";
	action.m_ActionMsg = "æå‰è¾ˆï¼Œæˆ‘è¿™å°±èµ¶å»ã€‚";
	return action;
end

function Task_00000017_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "<name>çœŸæ˜¯å¤ªæ„Ÿè°¢ä½ äº†ï¼Œè¿™è›‡å¦–çš„æ¯’é›¾å¥½ç”Ÿå‰å®³ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000017_step_table = {
		[1] = Task_00000017_step_01,
		[10] = Task_00000017_step_10,
		};

function Task_00000017_step(step)
	if Task_00000017_step_table[step] ~= nil then
		return Task_00000017_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000017_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000017() then
		return false;
	end
	if not task:AcceptTask(17) then
		return false;
	end
	task:AddTaskStep(17);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000017_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(17) then
		return false;
	end


	player:AddExp(1111);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000017_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(17);
end
