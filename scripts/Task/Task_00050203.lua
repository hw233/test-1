--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00050203()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 20 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50203) or task:HasCompletedTask(50203) or task:HasSubmitedTask(50203) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00050203()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 20 then
		return false;
	end
	if task:HasAcceptedTask(50203) or task:HasCompletedTask(50203) or task:HasSubmitedTask(50203) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00050203()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50203) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00050203(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50203) == npcId and Task_Accept_00050203 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50203
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç¾å¥½çš„æ€å¿µ";
	elseif task:GetTaskSubmitNpc(50203) == npcId then
		if Task_Submit_00050203() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50203
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç¾å¥½çš„æ€å¿µ";
		elseif task:HasAcceptedTask(50203) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50203
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç¾å¥½çš„æ€å¿µ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00050203_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "æˆ‘çš„å‹‡å£«ï¼Œä½ èƒ½å†å¸®æˆ‘æŠŠè¿™ç›˜å›´æ£‹é€ç»™è’²æ˜Œæµ·-çœŸå¯‚æ®¿çš„ç”°å¿Œå—ï¼Ÿè¿™æ˜¯ä»–æœ€å–œæ¬¢çš„ç‰©å“ï¼Œå¹¶è½¬è¾¾æˆ‘å’Œç‹å¯¹ä»–çš„æ€å¿µã€‚";
	action.m_ActionMsg = "åœ¨ä¸‹éå¸¸æ„¿æ„ä¸ºæ‚¨å’Œç‹æ•ˆåŠ³ã€‚";
	return action;
end

function Task_00050203_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘çš„å‹‡å£«ï¼Œä½ å¾ˆæ•ˆå¿ äºé­”æ—ï¼Œæˆ‘ä¼šå¸®åŠ©ä½ å¾—åˆ°æ›´å¤šçš„åŠ›é‡ï¼Œè®©ä½ æˆé•¿æ›´å¿«ã€‚";
	action.m_ActionMsg = "èª“æ­»æ•ˆå¿ é­”æ—ã€‚";
	return action;
end

function Task_00050203_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "èª“æ­»æ•ˆå¿ é­”æ—ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050203_step_table = {
		[1] = Task_00050203_step_01,
		[2] = Task_00050203_step_02,
		[10] = Task_00050203_step_10,
		};

function Task_00050203_step(step)
	if Task_00050203_step_table[step] ~= nil then
		return Task_00050203_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00050203_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050203() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15101, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(50203) then
		return false;
	end
	task:AddTaskStep(50203);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00050203_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15101,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(50203) then
		return false;
	end

	package:DelItemAll(15101,1);

	player:AddExp(2000);
	player:getCoin(10000);
	player:getTael(120);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00050203_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15101,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50203);
end
