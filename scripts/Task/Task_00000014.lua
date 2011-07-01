--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000014()
	local player = GetPlayer();
	if player:GetLev() < 13 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(14) or task:HasCompletedTask(14) or task:HasSubmitedTask(14) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(13) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(13) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000014()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 13 then
		return false;
	end
	if task:HasAcceptedTask(14) or task:HasCompletedTask(14) or task:HasSubmitedTask(14) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(13) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(13) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000014()
	if GetPlayer():GetTaskMgr():HasCompletedTask(14) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000014(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(14) == npcId and Task_Accept_00000014 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 14
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ•£ä¿®æ‹›å‹Ÿ";
	elseif task:GetTaskSubmitNpc(14) == npcId then
		if Task_Submit_00000014() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 14
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ•£ä¿®æ‹›å‹Ÿ";
		elseif task:HasAcceptedTask(14) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 14
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ•£ä¿®æ‹›å‹Ÿ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000014_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å—¯ï¼Œä¸€ä¸ªäººè¡Œèµ°æ±Ÿæ¹–ç¡®å®æœ‰äº›å±é™©ï¼Œä¿—è¯è¯´åŒæ‹³éš¾æ•Œå››æ‰‹ï¼Œæˆ‘è¿™å°±ç»™ä½ æ¨èä¸€äº›çŸ¥åäººå£«ï¼Œæœ‰ä¸ªä¼´è¡Œèµ°æ±Ÿæ¹–ä¹Ÿçƒ­é—¹å¾ˆå¤šå•Šã€‚";
	action.m_ActionMsg = "é‚£å°±æ‹œæ‰˜æŒæŸœä½ äº†ã€‚";
	return action;
end

function Task_00000014_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä¸é”™ï¼Œä¸é”™ï¼Œå…³äºæ¯›å¤ªçš„äº‹æƒ…æˆ‘å›å¤´å’Œä½ ç»†è¯´ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000014_step_table = {
		[1] = Task_00000014_step_01,
		[10] = Task_00000014_step_10,
		};

function Task_00000014_step(step)
	if Task_00000014_step_table[step] ~= nil then
		return Task_00000014_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000014_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000014() then
		return false;
	end
	if not task:AcceptTask(14) then
		return false;
	end
	task:AddTaskStep(14);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000014_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(14) then
		return false;
	end


	player:AddExp(1111);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000014_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(14);
end
