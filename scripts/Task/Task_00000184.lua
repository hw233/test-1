--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000184()
	local player = GetPlayer();
	if player:GetLev() < 90 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(184) or task:HasCompletedTask(184) or task:HasSubmitedTask(184) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000184()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 90 then
		return false;
	end
	if task:HasAcceptedTask(184) or task:HasCompletedTask(184) or task:HasSubmitedTask(184) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000184()
	if GetPlayer():GetTaskMgr():HasCompletedTask(184) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000184(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(184) == npcId and Task_Accept_00000184 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 184
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "èŠ¬é™€ç¥å°¼";
	elseif task:GetTaskSubmitNpc(184) == npcId then
		if Task_Submit_00000184() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 184
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "èŠ¬é™€ç¥å°¼";
		elseif task:HasAcceptedTask(184) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 184
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "èŠ¬é™€ç¥å°¼";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000184_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ å»æ˜†ä»‘è§è§èŠ¬é™€ç¥å°¼å§ï¼Œå¥¹å¯æ˜¯å‰è¾ˆé«˜äººã€‚";
	action.m_ActionMsg = "æ˜¯å—ï¼Œæˆ‘è¿™å°±å»æ˜†ä»‘å±±ã€‚";
	return action;
end

function Task_00000184_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ çœŸæ˜¯ä¿®ä¸ºæ·±åšï¼Œå¤©èµ‹å¼‚ç¦€å•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000184_step_table = {
		[1] = Task_00000184_step_01,
		[10] = Task_00000184_step_10,
		};

function Task_00000184_step(step)
	if Task_00000184_step_table[step] ~= nil then
		return Task_00000184_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000184_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000184() then
		return false;
	end
	if not task:AcceptTask(184) then
		return false;
	end
	task:AddTaskStep(184);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000184_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(184) then
		return false;
	end


	player:AddExp(150000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000184_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(184);
end
