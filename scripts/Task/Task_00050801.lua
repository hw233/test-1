--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00050801()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 25 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50801) or task:HasCompletedTask(50801) or task:HasSubmitedTask(50801) then
		return false;
	end
	if not task:HasSubmitedTask(50703) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00050801()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 25 then
		return false;
	end
	if task:HasAcceptedTask(50801) or task:HasCompletedTask(50801) or task:HasSubmitedTask(50801) then
		return false;
	end
	if not task:HasSubmitedTask(50703) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00050801()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50801) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00050801(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50801) == npcId and Task_Accept_00050801 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50801
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "éƒ­å­ä»ªæ±‚åŠ©";
	elseif task:GetTaskSubmitNpc(50801) == npcId then
		if Task_Submit_00050801() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50801
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "éƒ­å­ä»ªæ±‚åŠ©";
		elseif task:HasAcceptedTask(50801) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50801
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "éƒ­å­ä»ªæ±‚åŠ©";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00050801_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "å‹‡å£«ï¼Œå¦‚æœä¸ä»‹æ„ï¼Œå¯ä»¥é—®é—®ä½ ï¼Œä½ æ˜¯é­”æ—çš„å—ï¼Ÿ";
	action.m_ActionMsg = "æ˜¯å•Šï¼Œä»€ä¹ˆäº‹æƒ…ï¼Ÿ";
	return action;
end

function Task_00050801_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ˜¯é­”æ—çš„å°±å¥½ï¼Œæˆ‘æ­£çŠ¹è±«ä¸çŸ¥é“è¯¥æ‰¾è°å¸®å¿™ï¼Œéƒ½è¯´é­”æ—çš„å‹‡å£«å¾ˆå€¼å¾—äººä¿¡ä»»ï¼Œå¦‚æœä½ æ„¿æ„æ¥å¸®æˆ‘ï¼Œæˆ‘ä¹Ÿä¸ä¼šè®©ä½ ç©ºæ‰‹è€Œå½’çš„ã€‚";
	action.m_ActionMsg = "æ—¢ç„¶ä¸æ˜¯ç™½å¸®å¿™ï¼Œæˆ‘å€’æ˜¯å¯ä»¥è¯•è¯•çœ‹ã€‚";
	return action;
end

function Task_00050801_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "åˆ«æäº†ï¼Œæœ€è¿‘çœŸæ˜¯å¤ªèƒŒäº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050801_step_table = {
		[1] = Task_00050801_step_01,
		[2] = Task_00050801_step_02,
		[10] = Task_00050801_step_10,
		};

function Task_00050801_step(step)
	if Task_00050801_step_table[step] ~= nil then
		return Task_00050801_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00050801_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050801() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50801) then
		return false;
	end
	task:AddTaskStep(50801);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00050801_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(50801) then
		return false;
	end


	player:AddExp(3000);
	player:getCoin(4000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00050801_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50801);
end
