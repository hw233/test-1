--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00030904()
	local player = GetPlayer();
	if player:GetLev() < 55 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30904) or task:HasCompletedTask(30904) or task:HasSubmitedTask(30904) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30903) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30903) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00030904()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 55 then
		return false;
	end
	if task:HasAcceptedTask(30904) or task:HasCompletedTask(30904) or task:HasSubmitedTask(30904) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30903) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30903) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00030904()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30904) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00030904(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30904) == npcId and Task_Accept_00030904 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30904
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å®¡é—®æ‘©ä¼½";
	elseif task:GetTaskSubmitNpc(30904) == npcId then
		if Task_Submit_00030904() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30904
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å®¡é—®æ‘©ä¼½";
		elseif task:HasAcceptedTask(30904) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30904
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å®¡é—®æ‘©ä¼½";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00030904_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "çœ‹æ¥ç‹¡çŒ¾çš„æ‘©å¯æ‹¿åœ¨æˆ‘ä»¬å‘ç°ä»–ä¹‹å‰ï¼Œå°±å·²ç»é€ƒèµ°äº†ã€‚";
	action.m_ActionMsg = "æˆ‘æƒ³æ˜¯çš„ã€‚";
	return action;
end

function Task_00030904_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½†æ˜¯é‚£ä¸ªæ‘©ä¼½è‚¯å®šä¸æ˜¯ä»€ä¹ˆå¥½ä¸œè¥¿ï¼Œä½ ç°åœ¨å»å®¡é—®ä»–ï¼Œä¸€å®šä¸èƒ½è½»æ˜“æ”¾è¿‡ã€‚";
	action.m_ActionMsg = "å“¼å“¼ï¼Œä»–è¦æ˜¯æ•¢ä¸è¯´å®è¯ï¼Œæˆ‘ç ä¸‹ä»–çš„èˆŒå¤´ã€‚";
	return action;
end

function Task_00030904_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ€ä¹ˆåˆæœ‰ç”Ÿäººæ¥æ‰¾æˆ‘äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030904_step_table = {
		[1] = Task_00030904_step_01,
		[2] = Task_00030904_step_02,
		[10] = Task_00030904_step_10,
		};

function Task_00030904_step(step)
	if Task_00030904_step_table[step] ~= nil then
		return Task_00030904_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00030904_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030904() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30904) then
		return false;
	end
	task:AddTaskStep(30904);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00030904_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(505,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30904) then
		return false;
	end

	if IsEquipTypeId(505) then
		for k = 1, 1 do
			package:AddEquip(505, 1);
		end
	else 
		package:AddItem(505,1,1);
	end

	player:AddExp(76000);
	player:getCoin(70000);
	player:getTael(20);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00030904_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30904);
end
