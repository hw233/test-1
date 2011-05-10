--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00061602()
	local player = GetPlayer();
	if player:GetLev() < 61 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61602) or task:HasCompletedTask(61602) or task:HasSubmitedTask(61602) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61601) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61601) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00061602()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 61 then
		return false;
	end
	if task:HasAcceptedTask(61602) or task:HasCompletedTask(61602) or task:HasSubmitedTask(61602) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61601) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61601) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00061602()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61602) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00061602(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61602) == npcId and Task_Accept_00061602 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61602
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ‘©çºçš„ç›®çš„";
	elseif task:GetTaskSubmitNpc(61602) == npcId then
		if Task_Submit_00061602() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61602
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ‘©çºçš„ç›®çš„";
		elseif task:HasAcceptedTask(61602) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61602
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ‘©çºçš„ç›®çš„";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00061602_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "è¿™ä½å‹‡å£«å¸¦ç€è¿™å°å†Œå­æ¥æ‰¾æˆ‘ï¼Œæ˜¯ä»€ä¹ˆç”¨æ„å•Šï¼Ÿ";
	action.m_ActionMsg = "ä½ æ˜¯åœ¨è£…å—ï¼Ÿè¿™å°å†Œå­ä¸å°±æ˜¯ä½ å†™çš„å—ï¼Ÿ";
	return action;
end

function Task_00061602_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ˜¯æˆ‘å†™çš„åˆæ€ä¹ˆæ ·å‘¢ï¼Ÿ";
	action.m_ActionMsg = "ä½ æ˜¯æƒ³åˆ©ç”¨è¿™äº›å·¨äººï¼Œæ•™ä»–ä»¬é‚ªæ¶çš„è¡Œä¸ºè€Œè¾¾åˆ°æ§åˆ¶ä»–ä»¬çš„ç›®çš„å§ã€‚";
	return action;
end

function Task_00061602_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ æ€ä¹ˆèƒ½è¿™ä¹ˆè¯´ï¼Œè¿™æ˜¯æ¯«æ— æ ¹æ®çš„å§ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061602_step_table = {
		[1] = Task_00061602_step_01,
		[2] = Task_00061602_step_02,
		[10] = Task_00061602_step_10,
		};

function Task_00061602_step(step)
	if Task_00061602_step_table[step] ~= nil then
		return Task_00061602_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00061602_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061602() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61602) then
		return false;
	end
	task:AddTaskStep(61602);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00061602_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(61602) then
		return false;
	end


	player:AddExp(135000);
	player:getCoin(115000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00061602_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61602);
end
