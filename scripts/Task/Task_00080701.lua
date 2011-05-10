--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080701()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80701) or task:HasCompletedTask(80701) or task:HasSubmitedTask(80701) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00080701()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80701) or task:HasCompletedTask(80701) or task:HasSubmitedTask(80701) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00080701()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80701) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080701(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80701) == npcId and Task_Accept_00080701 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80701
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç„å¹»è‰";
	elseif task:GetTaskSubmitNpc(80701) == npcId then
		if Task_Submit_00080701() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80701
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç„å¹»è‰";
		elseif task:HasAcceptedTask(80701) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80701
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç„å¹»è‰";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080701_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "è®©é‚£äº›æ€ªç‰©è§é¬¼å»å§ï¼Œæˆ‘å·²ç»å—å¤Ÿäº†ä»–ä»¬ã€‚";
	action.m_ActionMsg = "æˆ‘ä»¬ä¸èƒ½å¿ƒæµ®æ°”èºï¼Œå¾—æƒ³æƒ³åŠæ³•ã€‚";
	return action;
end

function Task_00080701_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ ç°åœ¨å»ç„å¹»å¡é‡‡é›†ä¸€æ ªç„å¹»è‰æ¥ï¼Œæˆ‘æƒ³æˆ‘ä»¬è¯¥è¡ŒåŠ¨äº†ã€‚";
	action.m_ActionMsg = "ç„å¹»è‰ï¼Ÿé‚£èƒ½ç®¡ä»€ä¹ˆç”¨ï¼Ÿ";
	return action;
end

function Task_00080701_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™å°±æ˜¯ä¼ è¯´ä¸­ç¥å¥‡çš„ç„å¹»è‰å—ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080701_step_table = {
		[1] = Task_00080701_step_01,
		[2] = Task_00080701_step_02,
		[10] = Task_00080701_step_10,
		};

function Task_00080701_step(step)
	if Task_00080701_step_table[step] ~= nil then
		return Task_00080701_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080701_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080701() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80701) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15103,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15103, itemNum, 1);
		end
		task:AddTaskStep2(80701, 1, itemNum);
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00080701_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15103,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(80701) then
		return false;
	end

	package:DelItemAll(15103,1);

	player:AddExp(675000);
	player:getCoin(314000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080701_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15103,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80701);
end
