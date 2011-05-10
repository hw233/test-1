--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080801()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80801) or task:HasCompletedTask(80801) or task:HasSubmitedTask(80801) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00080801()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80801) or task:HasCompletedTask(80801) or task:HasSubmitedTask(80801) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00080801()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80801) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080801(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80801) == npcId and Task_Accept_00080801 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80801
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ä¸‘é™‹çš„è´è¶";
	elseif task:GetTaskSubmitNpc(80801) == npcId then
		if Task_Submit_00080801() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80801
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ä¸‘é™‹çš„è´è¶";
		elseif task:HasAcceptedTask(80801) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80801
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ä¸‘é™‹çš„è´è¶";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080801_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "ä½ å¬è¯´è¿‡æ¯æœ¨é•‡å—ï¼Ÿé‚£æ˜¯ä¸€ä¸ªæ‚²å“€çš„åœ°æ–¹ï¼Œæå…¶å‡„å‡‰ã€‚";
	action.m_ActionMsg = "æ˜¯ä»€ä¹ˆè®©é‚£é‡Œå˜æˆäº†è¿™æ ·ï¼Ÿ";
	return action;
end

function Task_00080801_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä¸çŸ¥é“ï¼Œé‚£é‡Œåªæœ‰æ¯èçš„æ ‘æœ¨å’Œæ—©å·²å¤±å»äº†è¡€è‰²çš„ç”Ÿå‘½ã€‚ä¼°è®¡æ˜¯è¢«è¯…å’’äº†ï¼Œä½ å»ç»“æŸå®ƒä»¬ç—›è‹¦çš„ä½™ç”Ÿå§ã€‚";
	action.m_ActionMsg = "å¬æ¥ç¡®å®æ‚²å‡‰ã€‚";
	return action;
end

function Task_00080801_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ‚²å‰§çš„äººç”Ÿä¹Ÿæ˜¯éœ€è¦æˆ‘ä»¬å»ç»å†çš„ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080801_step_table = {
		[1] = Task_00080801_step_01,
		[2] = Task_00080801_step_02,
		[10] = Task_00080801_step_10,
		};

function Task_00080801_step(step)
	if Task_00080801_step_table[step] ~= nil then
		return Task_00080801_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080801_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080801() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80801) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00080801_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80801) then
		return false;
	end


	player:AddExp(725000);
	player:getCoin(318000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080801_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80801);
end
