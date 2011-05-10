--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00020201()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20201) or task:HasCompletedTask(20201) or task:HasSubmitedTask(20201) then
		return false;
	end
	if not task:HasSubmitedTask(20106) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00020201()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(20201) or task:HasCompletedTask(20201) or task:HasSubmitedTask(20201) then
		return false;
	end
	if not task:HasSubmitedTask(20106) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00020201()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20201) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00020201(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20201) == npcId and Task_Accept_00020201 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20201
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "åˆè§æˆ˜ç¥";
	elseif task:GetTaskSubmitNpc(20201) == npcId then
		if Task_Submit_00020201() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20201
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "åˆè§æˆ˜ç¥";
		elseif task:HasAcceptedTask(20201) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20201
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "åˆè§æˆ˜ç¥";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00020201_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‹‡å£«æˆ‘æ­£å¥½éœ€è¦ä½ å»å¸®æˆ‘åšç‚¹äº‹æƒ…ï¼Œæˆ‘å¬è¯´ä¹‹å‰æ½œå…¥å¤©åº­çš„å¯†ä½¿æœ€è¿‘å¸¦å›å…¬ä¸»çš„æ¶ˆæ¯äº†ï¼Œä½ å¸®æˆ‘å»åŸå¤–æ‰¾åˆ°é­”æ—å¯†ä½¿ï¼Œçœ‹ä»–å¸¦äº†ä»€ä¹ˆæ¶ˆæ¯å›æ¥ã€‚";
	action.m_ActionMsg = "å“¦â€¦â€¦åˆè®©æˆ‘å»é€å£ä¿¡äº†ã€‚";
	return action;
end

function Task_00020201_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ æ˜¯è°ï¼Ÿæˆ‘ä»æ¥æ²¡æœ‰è§è¿‡ä½ ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020201_step_table = {
		[1] = Task_00020201_step_01,
		[10] = Task_00020201_step_10,
		};

function Task_00020201_step(step)
	if Task_00020201_step_table[step] ~= nil then
		return Task_00020201_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00020201_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020201() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20201) then
		return false;
	end
	task:AddTaskStep(20201);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00020201_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(20201) then
		return false;
	end


	player:AddExp(1400);
	player:getCoin(800);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00020201_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20201);
end
