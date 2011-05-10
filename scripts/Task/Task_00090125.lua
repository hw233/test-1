--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090125()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90125) or task:HasCompletedTask(90125) or task:HasSubmitedTask(90125) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090125()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90125) or task:HasCompletedTask(90125) or task:HasSubmitedTask(90125) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090125()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90125) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090125(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90125) == npcId and Task_Accept_00090125 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90125
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æœ€æ–°æˆ˜å†µ";
	elseif task:GetTaskSubmitNpc(90125) == npcId then
		if Task_Submit_00090125() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90125
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æœ€æ–°æˆ˜å†µ";
		elseif task:HasAcceptedTask(90125) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90125
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æœ€æ–°æˆ˜å†µ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090125_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‹‡å£«ï¼Œè¿™æ¬¡å¤šè°¢ä½ äº†ï¼Œè¯·ä½ å»å‘å°†å†›æ±‡æŠ¥æˆ‘è¿™çš„æˆ˜å†µï¼Œè®©ä»–æ”¾å¿ƒã€‚";
	action.m_ActionMsg = "å¥½çš„ï¼Œä½ ä¹Ÿè¦å°å¿ƒï¼Œé‚£äº›å¦–æ€ªä¸æ˜¯å¥½æƒ¹çš„ã€‚";
	return action;
end

function Task_00090125_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "çœ‹æ¥éº»çƒ¦å·²ç»è§£å†³äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090125_step_table = {
		[1] = Task_00090125_step_01,
		[10] = Task_00090125_step_10,
		};

function Task_00090125_step(step)
	if Task_00090125_step_table[step] ~= nil then
		return Task_00090125_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090125_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090125() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90125) then
		return false;
	end
	task:AddTaskStep(90125);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090125_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90125) then
		return false;
	end


	player:AddExp(725000);
	player:getCoin(318000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090125_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90125);
end
