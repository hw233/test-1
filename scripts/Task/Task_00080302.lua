--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080302()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80302) or task:HasCompletedTask(80302) or (task:HasSubmitedTask(80302) and (GetSharpDay(task:GetTaskEndTime(80302)) == GetSharpDay(os.time()))) then 
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00080302()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(80302) or task:HasCompletedTask(80302) or (task:HasSubmitedTask(80302) and (GetSharpDay(task:GetTaskEndTime(80302)) == GetSharpDay(os.time()))) then 
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00080302()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80302) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080302(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80302) == npcId and Task_Accept_00080302 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80302
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å³¥åµ˜å›½æˆ˜";
	elseif task:GetTaskSubmitNpc(80302) == npcId then
		if Task_Submit_00080302() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80302
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å³¥åµ˜å›½æˆ˜";
		elseif task:HasAcceptedTask(80302) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80302
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å³¥åµ˜å›½æˆ˜";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080302_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æƒ³è¦åšé•¿ä¹…çš„è‹±é›„æ˜¯å¾ˆéš¾çš„ï¼Œé£æ°´æ€»æ˜¯è½®æµè½¬ã€‚";
	action.m_ActionMsg = "å°±å‡­å¤©æ—é‚£è´ªå©ªçš„å˜´è„¸ï¼Ÿåªæ˜¯ç”˜éœ²ç»™äº†ä»–ä»¬å¨åŠ›ï¼Œæˆ‘å»ç­äº†ä»–ä»¬çš„å¨é£ã€‚";
	return action;
end

function Task_00080302_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æœç„¶æˆ‘æ²¡çœ‹é”™äººã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080302_step_table = {
		[1] = Task_00080302_step_01,
		[10] = Task_00080302_step_10,
		};

function Task_00080302_step(step)
	if Task_00080302_step_table[step] ~= nil then
		return Task_00080302_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080302_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080302() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80302) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00080302_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80302) then
		return false;
	end


	player:AddExp(50000);
	player:getTael(100);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080302_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80302);
end
