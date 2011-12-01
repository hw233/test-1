--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000161()
	local player = GetPlayer();
	if player:GetLev() < 75 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(161) or task:HasCompletedTask(161) or task:HasSubmitedTask(161) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(160) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(160) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(160) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000161()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 75 then
		return false;
	end
	if task:HasAcceptedTask(161) or task:HasCompletedTask(161) or task:HasSubmitedTask(161) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(160) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(160) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(160) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000161()
	if GetPlayer():GetTaskMgr():HasCompletedTask(161) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000161(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(161) == npcId and Task_Accept_00000161 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 161
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç„é¾Ÿæ®¿";
	elseif task:GetTaskSubmitNpc(161) == npcId then
		if Task_Submit_00000161() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 161
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç„é¾Ÿæ®¿";
		elseif task:HasAcceptedTask(161) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 161
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç„é¾Ÿæ®¿";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000161_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç´«äº‘å®«ä¸€å½¹ï¼Œè¯¸ä½å°‘ä¾ è”æ‰‹å¤§ç ´é‚ªæ´¾å¦–äººï¼Œå…¶ä¸­æ˜“æ°å…„å¼Ÿçš„ä¹å¤©ååœ°è¾Ÿé­”ç¥æ¢­ç¥å‡ºé¬¼æ²¡ï¼Œä¹Ÿç«‹äº†è«å¤§çš„åŠŸåŠ³ï¼Œè¿™è¾Ÿé­”ç¥æ¢­ä¹ƒæ˜¯ç„é¾Ÿæ®¿æ˜“å‘¨å‰è¾ˆæ‰€åˆ¶ï¼Œç«¯çš„æ˜¯ä¸€ä»¶å¼‚å®ã€‚é‚£ç„é¾Ÿæ®¿ç¦»æ­¤å¤„ä¸è¿œï¼Œä¸å¦‚å°‘ä¾ å»ç„é¾Ÿæ®¿è§è§å‰è¾ˆã€‚";
	action.m_ActionMsg = "å—¯ï¼Œæˆ‘è¿™å°±å»è§æ˜“å‘¨å‰è¾ˆã€‚";
	return action;
end

function Task_00000161_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä¹…é—»å°‘ä¾ ä¹‹åï¼Œå¦‚ä»Šä¸€è§ç¡®æ˜¯åä¸è™šä¼ ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000161_step_table = {
		[1] = Task_00000161_step_01,
		[10] = Task_00000161_step_10,
		};

function Task_00000161_step(step)
	if Task_00000161_step_table[step] ~= nil then
		return Task_00000161_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000161_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000161() then
		return false;
	end
	if not task:AcceptTask(161) then
		return false;
	end
	task:AddTaskStep(161);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000161_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(161) then
		return false;
	end


	player:AddExp(80000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000161_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(161);
end
