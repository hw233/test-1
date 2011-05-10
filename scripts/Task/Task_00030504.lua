--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00030504()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30504) or task:HasCompletedTask(30504) or task:HasSubmitedTask(30504) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30503) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30503) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00030504()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(30504) or task:HasCompletedTask(30504) or task:HasSubmitedTask(30504) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30503) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30503) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00030504()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30504) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00030504(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30504) == npcId and Task_Accept_00030504 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30504
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ä¿æŠ¤éšå½¢";
	elseif task:GetTaskSubmitNpc(30504) == npcId then
		if Task_Submit_00030504() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30504
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ä¿æŠ¤éšå½¢";
		elseif task:HasAcceptedTask(30504) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30504
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ä¿æŠ¤éšå½¢";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00030504_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ„Ÿè°¢ä½ ä¸ºæˆ‘ä»¬åšçš„è¿™ä¸€åˆ‡å‹‡å£«ï¼Œä¸œå±±æ´¾ä¸ºæœ‰ä½ è¿™æ ·çš„æœ‹å‹æ„Ÿåˆ°è£å¹¸ï¼Œè¯·ä½ å›åˆ°å±±æ²³éƒ¡çš„æ²³å·æ—…åº—è§éšå½¢ä¸€ä¸‹å§ï¼Œç¡®ä¿å¥¹æ²¡æœ‰é­åˆ°é‚£äº›åƒµå°¸åˆºå®¢çš„å·è¢­ï¼Œå’±ä»¬åä¼šæœ‰æœŸã€‚";
	action.m_ActionMsg = "å¥½çš„ï¼Œå¸®ä¸»å†ä¼šã€‚";
	return action;
end

function Task_00030504_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‹‡å£«è¿™ä¹ˆæ€¥åŒ†åŒ†çš„å°±å›æ¥äº†ï¼Œæœ‰ä»€ä¹ˆæ€¥äº‹å—ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030504_step_table = {
		[1] = Task_00030504_step_01,
		[10] = Task_00030504_step_10,
		};

function Task_00030504_step(step)
	if Task_00030504_step_table[step] ~= nil then
		return Task_00030504_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00030504_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030504() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30504) then
		return false;
	end
	task:AddTaskStep(30504);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00030504_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8925,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30504) then
		return false;
	end

	if IsEquipTypeId(8925) then
		for k = 1, 1 do
			package:AddEquip(8925, 1);
		end
	else 
		package:AddItem(8925,1,1);
	end

	player:AddExp(10000);
	player:getCoin(13500);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00030504_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30504);
end
