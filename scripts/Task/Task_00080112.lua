--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080112()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80112) or task:HasCompletedTask(80112) or (task:HasSubmitedTask(80112) and (GetSharpDay(task:GetTaskEndTime(80112)) == GetSharpDay(os.time()))) then 
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00080112()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(80112) or task:HasCompletedTask(80112) or (task:HasSubmitedTask(80112) and (GetSharpDay(task:GetTaskEndTime(80112)) == GetSharpDay(os.time()))) then 
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00080112()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80112) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080112(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80112) == npcId and Task_Accept_00080112 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80112
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "åå°†å—œå¥½";
	elseif task:GetTaskSubmitNpc(80112) == npcId then
		if Task_Submit_00080112() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80112
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "åå°†å—œå¥½";
		elseif task:HasAcceptedTask(80112) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80112
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "åå°†å—œå¥½";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080112_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "æ±Ÿæ¹–é™©æ¶ï¼Œä½ éœ€è¦å»ç»“äº¤ä¸€äº›åå°†ï¼Œä»–ä»¬å„è‡ªæœ‰ç€ä¸åŒçš„èƒ½åŠ›èƒ½å¸®åŠ©ä½ ä½œæˆ˜ã€‚ä½†é¦–å…ˆéœ€è¦ä½ èŠ±äº›åŠŸå¤«å»æ‰¾ä»–ä»¬å–œæ¬¢çš„ä¸œè¥¿ï¼Œç„¶åé€ç»™ä»–ä»¬ä»¥è¡¨è¯šæ„ã€‚";
	action.m_ActionMsg = "åƒé‡Œéš¾å¯»æ˜¯æœ‹å‹ï¼Œæˆ‘å¯ä»¥ç»™ä¸åŒçš„åå°†é€ä¸œè¥¿å—ï¼Ÿ";
	return action;
end

function Task_00080112_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å½“ç„¶å¯ä»¥ï¼Œæœ‹å‹å¤šäº†è·¯å¥½èµ°å˜›ã€‚";
	action.m_ActionMsg = "é‚£æˆ‘å¾—èµ¶ç´§å»å¯»æ‰¾ä»–ä»¬å–œæ¬¢çš„ä¸œè¥¿ã€‚";
	return action;
end

function Task_00080112_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ ç»“äº¤åˆ°æœ‹å‹äº†å—ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080112_step_table = {
		[1] = Task_00080112_step_01,
		[2] = Task_00080112_step_02,
		[10] = Task_00080112_step_10,
		};

function Task_00080112_step(step)
	if Task_00080112_step_table[step] ~= nil then
		return Task_00080112_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080112_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080112() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80112) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00080112_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80112) then
		return false;
	end


	player:AddExp(20000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080112_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80112);
end
