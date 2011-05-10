--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080202()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80202) or task:HasCompletedTask(80202) or (task:HasSubmitedTask(80202) and (GetSharpDay(task:GetTaskEndTime(80202)) == GetSharpDay(os.time()))) then 
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00080202()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(80202) or task:HasCompletedTask(80202) or (task:HasSubmitedTask(80202) and (GetSharpDay(task:GetTaskEndTime(80202)) == GetSharpDay(os.time()))) then 
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00080202()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80202) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080202(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80202) == npcId and Task_Accept_00080202 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80202
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å³¥åµ˜å›½æˆ˜";
	elseif task:GetTaskSubmitNpc(80202) == npcId then
		if Task_Submit_00080202() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80202
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å³¥åµ˜å›½æˆ˜";
		elseif task:HasAcceptedTask(80202) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80202
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
function Task_00080202_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æƒ³è¦åšé•¿ä¹…çš„è‹±é›„æ˜¯å¾ˆéš¾çš„ï¼Œé£æ°´æ€»æ˜¯è½®æµè½¬ã€‚";
	action.m_ActionMsg = "å°±å‡­é­”æ—é‚£äº›ä¸‘å…«æ€ªï¼Ÿæ²¡æœ‰ç”˜éœ²çš„æ°‘æ—éš¾æˆå¤§æ°”ï¼Œæˆ‘è¿™å°±å»ç­äº†ä»–ä»¬çš„å¨é£ã€‚";
	return action;
end

function Task_00080202_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æœç„¶æˆ‘æ²¡çœ‹é”™äººã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080202_step_table = {
		[1] = Task_00080202_step_01,
		[10] = Task_00080202_step_10,
		};

function Task_00080202_step(step)
	if Task_00080202_step_table[step] ~= nil then
		return Task_00080202_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080202_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080202() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80202) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00080202_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80202) then
		return false;
	end


	player:AddExp(50000);
	player:getTael(100);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080202_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80202);
end
