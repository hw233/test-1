--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090106()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90106) or task:HasCompletedTask(90106) or task:HasSubmitedTask(90106) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090106()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90106) or task:HasCompletedTask(90106) or task:HasSubmitedTask(90106) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090106()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90106) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090106(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90106) == npcId and Task_Accept_00090106 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90106
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¯æ€œçš„æ— æš‡";
	elseif task:GetTaskSubmitNpc(90106) == npcId then
		if Task_Submit_00090106() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90106
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¯æ€œçš„æ— æš‡";
		elseif task:HasAcceptedTask(90106) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90106
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¯æ€œçš„æ— æš‡";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090106_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ— æš‡æ˜¯ä¸ªå¯æ€œçš„å­©å­ï¼Œä¸€ç›´åˆ°ç°åœ¨éƒ½ä¸çŸ¥é“è‡ªå·±çš„ç”Ÿçˆ¶æ¯æ˜¯è°ã€‚æˆ‘ç°åœ¨åœ¨è¿™å„¿åŠ©æˆ˜ï¼ŒæŠ½ä¸å‡ºç©ºå»çœ‹å¥¹ï¼Œä½ æ›¿æˆ‘å»å§ã€‚";
	action.m_ActionMsg = "æ®è¯´å¥¹æ˜¯ä¸ªå¯äººå„¿ï¼Œè¿™ä¹ˆä¹–å·§çš„å¥³å„¿ï¼Œçˆ¶æ¯æ€èˆå¾—ä¸¢å¼ƒå‘€ã€‚";
	return action;
end

function Task_00090106_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ æ˜¯è°ï¼Ÿæˆ‘è®¤è¯†ä½ å—ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090106_step_table = {
		[1] = Task_00090106_step_01,
		[10] = Task_00090106_step_10,
		};

function Task_00090106_step(step)
	if Task_00090106_step_table[step] ~= nil then
		return Task_00090106_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090106_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090106() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90106) then
		return false;
	end
	task:AddTaskStep(90106);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090106_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90106) then
		return false;
	end


	player:AddExp(575000);
	player:getCoin(306000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090106_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90106);
end
