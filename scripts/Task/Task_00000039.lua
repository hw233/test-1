--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000039()
	local player = GetPlayer();
	if player:GetLev() < 28 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(39) or task:HasCompletedTask(39) or task:HasSubmitedTask(39) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(38) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(38) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(38) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000039()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 28 then
		return false;
	end
	if task:HasAcceptedTask(39) or task:HasCompletedTask(39) or task:HasSubmitedTask(39) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(38) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(38) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(38) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000039()
	if GetPlayer():GetTaskMgr():HasCompletedTask(39) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000039(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(39) == npcId and Task_Accept_00000039 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 39
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ­¦ä¾¯ç¥ ";
	elseif task:GetTaskSubmitNpc(39) == npcId then
		if Task_Submit_00000039() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 39
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ­¦ä¾¯ç¥ ";
		elseif task:HasAcceptedTask(39) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 39
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ­¦ä¾¯ç¥ ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000039_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."æ–¹æ‰é‚£ä½é†‰é“äººæ¥æ‰¾è¿‡ä½ ï¼Œè®©æˆ‘è½¬å‘Šè¯´ä»–ä¼šåœ¨æˆéƒ½æ­¦ä¾¯ç¥ ç­‰ä½ ï¼Œè®©ä½ å°½å¿«èµ¶å»ã€‚";
	action.m_ActionMsg = "æˆ‘çŸ¥é“äº†ã€‚";
	return action;
end

function Task_00000039_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ï¼ˆnameï¼‰ä½ æœç„¶ä¾ å¿ƒä¹‰èƒ†ï¼Œæ˜¯æˆ‘æ­£é“ä¸­äººå•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000039_step_table = {
		[1] = Task_00000039_step_01,
		[10] = Task_00000039_step_10,
		};

function Task_00000039_step(step)
	if Task_00000039_step_table[step] ~= nil then
		return Task_00000039_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000039_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000039() then
		return false;
	end
	if not task:AcceptTask(39) then
		return false;
	end
	task:AddTaskStep(39);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000039_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(39) then
		return false;
	end


	player:AddExp(3333);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000039_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(39);
end
