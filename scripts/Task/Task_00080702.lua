--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080702()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80702) or task:HasCompletedTask(80702) or task:HasSubmitedTask(80702) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80701) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80701) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00080702()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80702) or task:HasCompletedTask(80702) or task:HasSubmitedTask(80702) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80701) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80701) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00080702()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80702) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080702(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80702) == npcId and Task_Accept_00080702 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80702
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç ”ç©¶å¹»æœ¯";
	elseif task:GetTaskSubmitNpc(80702) == npcId then
		if Task_Submit_00080702() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80702
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç ”ç©¶å¹»æœ¯";
		elseif task:HasAcceptedTask(80702) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80702
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç ”ç©¶å¹»æœ¯";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080702_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "è¿™å°±æ˜¯ä¼ è¯´ä¸­ç¥å¥‡çš„ç„å¹»è‰å—ï¼Ÿ";
	action.m_ActionMsg = "é‚£å®ƒæœ‰ä»€ä¹ˆæ•ˆæœå‘¢ï¼Ÿ";
	return action;
end

function Task_00080702_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘ä¹Ÿä¸æ¸…æ¥šï¼Œè¿™åº”è¯¥è·Ÿå¹»æœ¯æœ‰å…³ç³»ï¼Œä½ æ‹¿å»ç»™å·«å¸ˆï¼Œæˆ‘æƒ³å¥¹ä¼šçŸ¥é“ã€‚";
	action.m_ActionMsg = "å°±è¿™ä¹ˆä¸€æ ªå°è‰ï¼Œæˆ‘çœ‹ä½œç”¨ä¸å¤§ã€‚";
	return action;
end

function Task_00080702_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™å¯æ˜¯æéš¾å¾—çš„ç„å¹»è‰å‘€ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080702_step_table = {
		[1] = Task_00080702_step_01,
		[2] = Task_00080702_step_02,
		[10] = Task_00080702_step_10,
		};

function Task_00080702_step(step)
	if Task_00080702_step_table[step] ~= nil then
		return Task_00080702_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080702_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080702() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15103, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(80702) then
		return false;
	end
	task:AddTaskStep(80702);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00080702_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15103,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(80702) then
		return false;
	end

	package:DelItemAll(15103,1);

	player:AddExp(675000);
	player:getCoin(314000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080702_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15103,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80702);
end
