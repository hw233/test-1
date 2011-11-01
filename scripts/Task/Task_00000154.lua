--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000154()
	local player = GetPlayer();
	if player:GetLev() < 70 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(154) or task:HasCompletedTask(154) or task:HasSubmitedTask(154) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000154()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 70 then
		return false;
	end
	if task:HasAcceptedTask(154) or task:HasCompletedTask(154) or task:HasSubmitedTask(154) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000154()
	if GetPlayer():GetTaskMgr():HasCompletedTask(154) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000154(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(154) == npcId and Task_Accept_00000154 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 154
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç´«äº‘å®«";
	elseif task:GetTaskSubmitNpc(154) == npcId then
		if Task_Submit_00000154() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 154
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç´«äº‘å®«";
		elseif task:HasAcceptedTask(154) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 154
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç´«äº‘å®«";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000154_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç´«äº‘å®«åŸæ˜¯è¿å±±å¤§å¸ˆåˆ«åºœï¼Œå¤©ä¸€é‡‘æ¯çš„æ—§å±…ï¼Œè¿™ç´«äº‘ä¸‰å¥³å‰ä¸–ä¹ƒå¤©ä¸€é‡‘æ¯çš„ä¾å¥³ï¼Œæ­¤ç•ªè½¬ä¸–é‡æ¥ï¼Œå´è¯¯å…¥æ­§é€”ï¼Œå‹¾ç»“é‚ªé­”å¤–é“ã€‚è¿™å›ä»¤ä½ å»ä¸œæµ·ï¼Œå°±æ˜¯è¦å°†è¿™äº›é‚ªæ´¾äººå£«é“²é™¤ã€‚";
	action.m_ActionMsg = "å—¯ï¼Œæˆ‘è¿™å°±å»ã€‚";
	return action;
end

function Task_00000154_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç´«äº‘å®«åŸæ˜¯ä»™äººåºœé‚¸ï¼Œä½†æ˜¯ç°åœ¨å´è¢«è¿™ç¾¤å¦–äººå æ®ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000154_step_table = {
		[1] = Task_00000154_step_01,
		[10] = Task_00000154_step_10,
		};

function Task_00000154_step(step)
	if Task_00000154_step_table[step] ~= nil then
		return Task_00000154_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000154_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000154() then
		return false;
	end
	if not task:AcceptTask(154) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000154_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(154) then
		return false;
	end


	player:AddExp(80000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000154_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(154);
end
