--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090111()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90111) or task:HasCompletedTask(90111) or task:HasSubmitedTask(90111) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90110) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90110) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090111()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90111) or task:HasCompletedTask(90111) or task:HasSubmitedTask(90111) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90110) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90110) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090111()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90111) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090111(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90111) == npcId and Task_Accept_00090111 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90111
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "åŠå°†";
	elseif task:GetTaskSubmitNpc(90111) == npcId then
		if Task_Submit_00090111() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90111
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "åŠå°†";
		elseif task:HasAcceptedTask(90111) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90111
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "åŠå°†";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090111_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "æ˜¯å‹’å‹¤å°†å†›è®©ä½ æ¥æ¢æœ›å¦¾èº«çš„ï¼Ÿ";
	action.m_ActionMsg = "æ˜¯çš„ï¼Œä»–ååˆ†æƒ³å¿µå§‘å¨˜ï¼Œç‰¹é£åœ¨ä¸‹æ¥æ¢æœ›ã€‚";
	return action;
end

function Task_00090111_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‘µå‘µï¼Œæˆ‘çœ‹ä»–ä¸æ˜¯æƒ³å¿µæˆ‘ï¼Œæ˜¯æƒ³å¿µå•å¸ƒã€‚æˆ‘çŸ¥é“ä»–æƒ³å«å•å¸ƒå»å¤œæ‘©åŸï¼Œä½†æ˜¯å•å¸ƒæ˜¯ä½•ç­‰çš„å¿ƒé«˜æ°”å‚²ä¹‹äººï¼Œå“ªèƒ½å¬ä»–è°ƒé£ã€‚ä¸è¿‡ä½ å¯ä»¥å»è¯•è¯•ã€‚";
	action.m_ActionMsg = "æˆ‘æ˜¯å¥‰å‘½è€Œæ¥ï¼Œä¸èƒ½ç©ºæ‰‹è€Œå½’ï¼Œé‚£æˆ‘å»è¯•è¯•å§ã€‚";
	return action;
end

function Task_00090111_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "åŸæ¥ä½ æ˜¯æ¥åŠå°†çš„ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090111_step_table = {
		[1] = Task_00090111_step_01,
		[2] = Task_00090111_step_02,
		[10] = Task_00090111_step_10,
		};

function Task_00090111_step(step)
	if Task_00090111_step_table[step] ~= nil then
		return Task_00090111_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090111_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090111() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90111) then
		return false;
	end
	task:AddTaskStep(90111);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090111_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90111) then
		return false;
	end


	player:AddExp(600000);
	player:getCoin(308000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090111_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90111);
end
