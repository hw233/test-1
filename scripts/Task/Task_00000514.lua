--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000514()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(514) or task:HasCompletedTask(514) or task:HasSubmitedTask(514) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000514()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(514) or task:HasCompletedTask(514) or task:HasSubmitedTask(514) then
		return false;
	end
	if player:ColorTaskOutOf(0) then
		return false
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000514()
	if GetPlayer():GetTaskMgr():HasCompletedTask(514) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000514(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(514) == npcId and Task_Accept_00000514 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 514
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "åµ©å±±äºŒè€";
	elseif task:GetTaskSubmitNpc(514) == npcId then
		if Task_Submit_00000514() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 514
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "åµ©å±±äºŒè€";
		elseif task:HasAcceptedTask(514) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 514
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "åµ©å±±äºŒè€";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000514_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "åµ©å±±äºŒè€ä¹ƒæ˜¯çŸ¥åçš„å‰‘ä¾ ï¼Œç¥é¾™è§é¦–ä¸è§å°¾å•Šï¼Œä¸è¿‡æ—¢ç„¶ä½ åœ¨æˆéƒ½éƒŠå¤–é‡è§è¿½äº‘åŸå‰è¾ˆï¼Œä¹Ÿæ˜¯ä¸€ç§ç¼˜åˆ†ï¼Œåº”è¯¥å¤šå¤šå»æ‹œè®¿ä»–ã€‚";
	action.m_ActionMsg = "å¼Ÿå­è¿™å°±å»ã€‚";
	return action;
end

function Task_00000514_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‘µå‘µï¼Œå°æœ‹å‹ï¼Œæˆ‘ä»¬åˆè§é¢äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000514_step_table = {
		[1] = Task_00000514_step_01,
		[10] = Task_00000514_step_10,
		};

function Task_00000514_step(step)
	if Task_00000514_step_table[step] ~= nil then
		return Task_00000514_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000514_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000514() then
		return false;
	end
	if not task:AcceptTask(514) then
		return false;
	end
	task:AddTaskStep(514);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000514_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(514) then
		return false;
	end


	player:AddExp(1111);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000514_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(514);
end
