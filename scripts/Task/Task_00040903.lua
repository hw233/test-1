--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00040903()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 28 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40903) or task:HasCompletedTask(40903) or task:HasSubmitedTask(40903) then
		return false;
	end
	if not task:HasSubmitedTask(40902) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00040903()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 28 then
		return false;
	end
	if task:HasAcceptedTask(40903) or task:HasCompletedTask(40903) or task:HasSubmitedTask(40903) then
		return false;
	end
	if not task:HasSubmitedTask(40902) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00040903()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40903) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00040903(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40903) == npcId and Task_Accept_00040903 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40903
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¿ƒæ— æ‚å¿µ";
	elseif task:GetTaskSubmitNpc(40903) == npcId then
		if Task_Submit_00040903() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40903
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¿ƒæ— æ‚å¿µ";
		elseif task:HasAcceptedTask(40903) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40903
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¿ƒæ— æ‚å¿µ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00040903_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä¸ºäº†æ£€éªŒä½ æ˜¯å¦çœŸçš„å·²ç»å¿ƒæ— æ‚å¿µï¼Œæ— è§†å‡¡å°˜ï¼Œä¸€å¿ƒè¿½æ±‚åŠ›é‡çš„æå‡ï¼Œæˆ‘ç°åœ¨éœ€è¦ä½ å»è¥¿æ¥å±±è°·ï¼Œæ‰“è´¥é‚£é‡Œçš„åƒµå°¸å°„æ‰‹ï¼Œæ—¢ä¸ºè¯æ˜ä½ è‡ªå·±ï¼Œä¹Ÿä¸ºæ¢å¤è§‚éŸ³å±±çš„å¹³é™å®‰å®šã€‚";
	action.m_ActionMsg = "æˆ‘å››å¤„å¾æˆ˜å¤šå¹´ï¼Œé‚£äº›åƒµå°¸æ ¹æœ¬ä¸æ˜¯æˆ‘çš„å¯¹æ‰‹ã€‚";
	return action;
end

function Task_00040903_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ åšäº‹æƒ…çœŸæ˜¯å¹²å‡€æ¼‚äº®ï¼Œè¿™ä¸€ç‚¹å¾ˆè®©äººæ¬£èµã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040903_step_table = {
		[1] = Task_00040903_step_01,
		[10] = Task_00040903_step_10,
		};

function Task_00040903_step(step)
	if Task_00040903_step_table[step] ~= nil then
		return Task_00040903_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00040903_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040903() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40903) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00040903_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40903) then
		return false;
	end


	player:AddExp(4500);
	player:getCoin(4900);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00040903_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40903);
end
