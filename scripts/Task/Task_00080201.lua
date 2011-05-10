--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080201()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80201) or task:HasCompletedTask(80201) or (task:HasSubmitedTask(80201) and (GetSharpDay(task:GetTaskEndTime(80201)) == GetSharpDay(os.time()))) then 
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00080201()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(80201) or task:HasCompletedTask(80201) or (task:HasSubmitedTask(80201) and (GetSharpDay(task:GetTaskEndTime(80201)) == GetSharpDay(os.time()))) then 
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00080201()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80201) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080201(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80201) == npcId and Task_Accept_00080201 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80201
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "åŒ¹å¤«ä¹‹è´£";
	elseif task:GetTaskSubmitNpc(80201) == npcId then
		if Task_Submit_00080201() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80201
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "åŒ¹å¤«ä¹‹è´£";
		elseif task:HasAcceptedTask(80201) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80201
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "åŒ¹å¤«ä¹‹è´£";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080201_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ˜äº‰å³å°†å¼€å§‹ï¼Œæˆ‘å‘½ä½ å‡ºæˆ˜ã€‚ç”¨ä½ çš„åŠ›é‡å»æ‰“è´¥æ•Œäººï¼Œå¤©æ—å°†ä»¥ä½ ä¸ºè£ã€‚";
	action.m_ActionMsg = "ç”Ÿæ˜¯å¤©æ—çš„äººï¼Œæ­»æ˜¯å¤©æ—çš„é­‚ã€‚è¯·ç‹æ”¾å¿ƒï¼Œæ­¤å»å¿…èƒœï¼";
	return action;
end

function Task_00080201_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¤©æ—ä»¥ä½ ä¸ºè£ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080201_step_table = {
		[1] = Task_00080201_step_01,
		[10] = Task_00080201_step_10,
		};

function Task_00080201_step(step)
	if Task_00080201_step_table[step] ~= nil then
		return Task_00080201_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080201_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080201() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80201) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00080201_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80201) then
		return false;
	end


	player:AddExp(25000);
	player:getTael(50);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080201_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80201);
end
