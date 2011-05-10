--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00061503()
	local player = GetPlayer();
	if player:GetLev() < 58 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61503) or task:HasCompletedTask(61503) or task:HasSubmitedTask(61503) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61502) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61502) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00061503()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 58 then
		return false;
	end
	if task:HasAcceptedTask(61503) or task:HasCompletedTask(61503) or task:HasSubmitedTask(61503) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61502) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61502) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00061503()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61503) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00061503(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61503) == npcId and Task_Accept_00061503 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61503
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ”¶æ‹¾åº­é™¢";
	elseif task:GetTaskSubmitNpc(61503) == npcId then
		if Task_Submit_00061503() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61503
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ”¶æ‹¾åº­é™¢";
		elseif task:HasAcceptedTask(61503) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61503
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ”¶æ‹¾åº­é™¢";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00061503_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "å‹‡å£«å¤šäºäº†ä½ å•Šï¼Œè¦ä¸æˆ‘åœ¨è¿™å†°å‡ŒåŸï¼Œä¸æ˜¯è¢«æ€ªç‰©æ‰“æ­»å°±æ˜¯è¢«å†»æ­»äº†ã€‚";
	action.m_ActionMsg = "ä½ è¿˜çœŸæ˜¯æŒºä¸å®¹æ˜“çš„ã€‚";
	return action;
end

function Task_00061503_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å—¯åˆ«æäº†ï¼Œå†éº»çƒ¦ä½ å»å‘Šè¯‰æˆ‘æ—è¾¹çš„å¦‚çƒŸçš„éšä»ï¼Œè·Ÿä»–è¯´é—¨æ¿å·²ç»å¼„å›æ¥äº†ï¼Œè¯·ä»–æŠŠè¿™ä¹±ç³Ÿç³Ÿçš„æ¸¡å†°å›­æ”¶æ‹¾æ•´ç†å¥½å§ã€‚";
	action.m_ActionMsg = "å¥½çš„ï¼Œæ²¡é—®é¢˜ã€‚";
	return action;
end

function Task_00061503_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¥½çš„å¥½çš„ï¼ŒçŸ¥é“äº†ï¼Œæˆ‘è¿™å°±å¼€å§‹æ”¶æ‹¾ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061503_step_table = {
		[1] = Task_00061503_step_01,
		[2] = Task_00061503_step_02,
		[10] = Task_00061503_step_10,
		};

function Task_00061503_step(step)
	if Task_00061503_step_table[step] ~= nil then
		return Task_00061503_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00061503_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061503() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61503) then
		return false;
	end
	task:AddTaskStep(61503);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00061503_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8913,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(61503) then
		return false;
	end

	if IsEquipTypeId(8913) then
		for k = 1, 1 do
			package:AddEquip(8913, 1);
		end
	else 
		package:AddItem(8913,1,1);
	end

	player:AddExp(88000);
	player:getCoin(90000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00061503_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61503);
end
