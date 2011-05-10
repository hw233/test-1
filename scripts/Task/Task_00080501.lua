--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080501()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80501) or task:HasCompletedTask(80501) or task:HasSubmitedTask(80501) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00080501()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80501) or task:HasCompletedTask(80501) or task:HasSubmitedTask(80501) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00080501()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80501) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080501(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80501) == npcId and Task_Accept_00080501 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80501
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ¢æœ›å‹’å‹¤";
	elseif task:GetTaskSubmitNpc(80501) == npcId then
		if Task_Submit_00080501() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80501
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ¢æœ›å‹’å‹¤";
		elseif task:HasAcceptedTask(80501) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80501
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ¢æœ›å‹’å‹¤";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080501_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ å¸®æˆ‘å»æ¢æœ›å¤œæ‘©åŸçš„å‹’å‹¤å§ï¼Œä»–å¯èƒ½ä¼šéœ€è¦ä½ çš„å¸®åŠ©ã€‚";
	action.m_ActionMsg = "å¬è¯´ä»–æ˜¯ä¸€ä½ç™¾æˆ˜ç™¾èƒœçš„å°†é¢†ã€‚";
	return action;
end

function Task_00080501_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ æ˜¯æ¥å¸®æˆ‘çš„å—ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080501_step_table = {
		[1] = Task_00080501_step_01,
		[10] = Task_00080501_step_10,
		};

function Task_00080501_step(step)
	if Task_00080501_step_table[step] ~= nil then
		return Task_00080501_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080501_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080501() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80501) then
		return false;
	end
	task:AddTaskStep(80501);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00080501_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80501) then
		return false;
	end


	player:AddExp(500000);
	player:getCoin(300000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080501_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80501);
end
