--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090104()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90104) or task:HasCompletedTask(90104) or task:HasSubmitedTask(90104) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90103) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90103) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090104()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90104) or task:HasCompletedTask(90104) or task:HasSubmitedTask(90104) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90103) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90103) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090104()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90104) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090104(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90104) == npcId and Task_Accept_00090104 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90104
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "åªæ¬ ä¸œé£";
	elseif task:GetTaskSubmitNpc(90104) == npcId then
		if Task_Submit_00090104() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90104
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "åªæ¬ ä¸œé£";
		elseif task:HasAcceptedTask(90104) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90104
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "åªæ¬ ä¸œé£";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090104_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "ä¸‡äº‹å…·å¤‡ï¼Œåªæ¬ ä¸œé£ã€‚";
	action.m_ActionMsg = "ä¸œé£æ˜¯ä»€ä¹ˆï¼Ÿ";
	return action;
end

function Task_00090104_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘åœ¨ç ”åˆ¶ä¸€ç§ç¥è¯ï¼Œå¯ä»¥å¢å¼ºå°†å£«ä»¬çš„æˆ˜æ–—åŠ›ã€‚ä½†æ˜¯è¿™è¯è¿˜ç¼ºæœ€åä¸€å‘³ææ–™ï¼Œé‚£ä¾¿æ˜¯ç„å¹»å¡çš„ç„å¹»è‰ï¼Œä½ å¸®æˆ‘å»å–æ¥å§ã€‚";
	action.m_ActionMsg = "è¡Œï¼Œä½ åœ¨è¿™ç­‰æˆ‘ã€‚";
	return action;
end

function Task_00090104_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘ç»ˆäºç ”åˆ¶å‡ºç¥è¯äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090104_step_table = {
		[1] = Task_00090104_step_01,
		[2] = Task_00090104_step_02,
		[10] = Task_00090104_step_10,
		};

function Task_00090104_step(step)
	if Task_00090104_step_table[step] ~= nil then
		return Task_00090104_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090104_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090104() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15103, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(90104) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15103,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15103, itemNum, 1);
		end
		task:AddTaskStep2(90104, 1, itemNum);
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090104_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15103,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(90104) then
		return false;
	end

	package:DelItemAll(15103,1);

	player:AddExp(550000);
	player:getCoin(304000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090104_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15103,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90104);
end
