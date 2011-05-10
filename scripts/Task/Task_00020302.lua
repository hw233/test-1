--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00020302()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 19 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20302) or task:HasCompletedTask(20302) or task:HasSubmitedTask(20302) then
		return false;
	end
	if not task:HasSubmitedTask(20301) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00020302()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 19 then
		return false;
	end
	if task:HasAcceptedTask(20302) or task:HasCompletedTask(20302) or task:HasSubmitedTask(20302) then
		return false;
	end
	if not task:HasSubmitedTask(20301) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00020302()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20302) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00020302(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20302) == npcId and Task_Accept_00020302 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20302
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ–‡éš£ä¹‹å¢“";
	elseif task:GetTaskSubmitNpc(20302) == npcId then
		if Task_Submit_00020302() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20302
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ–‡éš£ä¹‹å¢“";
		elseif task:HasAcceptedTask(20302) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20302
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ–‡éš£ä¹‹å¢“";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00020302_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ–‡éš£ä¹‹å¢“  é¦™éŸ³ç‹åæ•¬";
	action.m_ActionMsg = "éš¾é“è·Ÿé¦™éŸ³ç‹åæœ‰å…³ï¼Ÿçœ‹æ¥æˆ‘è¦å»æ‰¾ä¸€ä¸‹ç‹åã€‚";
	return action;
end

function Task_00020302_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‹‡å£«ï¼Œä½ æ˜¯ä¸ºä½•è€Œæ¥ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020302_step_table = {
		[1] = Task_00020302_step_01,
		[10] = Task_00020302_step_10,
		};

function Task_00020302_step(step)
	if Task_00020302_step_table[step] ~= nil then
		return Task_00020302_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00020302_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020302() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20302) then
		return false;
	end
	task:AddTaskStep(20302);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00020302_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(20302) then
		return false;
	end


	player:AddExp(1800);
	player:getCoin(1900);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00020302_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20302);
end
