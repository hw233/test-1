--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000511()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(511) or task:HasCompletedTask(511) or task:HasSubmitedTask(511) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000511()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(511) or task:HasCompletedTask(511) or task:HasSubmitedTask(511) then
		return false;
	end
	if player:ColorTaskOutOf(0) then
		return false
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000511()
	if GetPlayer():GetTaskMgr():HasCompletedTask(511) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000511(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(511) == npcId and Task_Accept_00000511 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 511
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æˆéƒ½çŸ¥åºœ";
	elseif task:GetTaskSubmitNpc(511) == npcId then
		if Task_Submit_00000511() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 511
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æˆéƒ½çŸ¥åºœ";
		elseif task:HasAcceptedTask(511) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 511
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æˆéƒ½çŸ¥åºœ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000511_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ å»æ‹œè®¿ä¸€ä¸‹æˆéƒ½çŸ¥åºœå§ã€‚";
	action.m_ActionMsg = "å¥½çš„ï¼Œå¼Ÿå­è¿™å°±å»ã€‚";
	return action;
end

function Task_00000511_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "çœŸæ˜¯ä¸æ•¢å½“å•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000511_step_table = {
		[1] = Task_00000511_step_01,
		[10] = Task_00000511_step_10,
		};

function Task_00000511_step(step)
	if Task_00000511_step_table[step] ~= nil then
		return Task_00000511_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000511_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000511() then
		return false;
	end
	if not task:AcceptTask(511) then
		return false;
	end
	task:AddTaskStep(511);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000511_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(511) then
		return false;
	end


	player:AddExp(1111);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000511_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(511);
end
