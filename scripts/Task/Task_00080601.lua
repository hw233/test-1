--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080601()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80601) or task:HasCompletedTask(80601) or task:HasSubmitedTask(80601) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00080601()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80601) or task:HasCompletedTask(80601) or task:HasSubmitedTask(80601) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00080601()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80601) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080601(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80601) == npcId and Task_Accept_00080601 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80601
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ä¾¦å¯ŸçŸ³æ¡¥";
	elseif task:GetTaskSubmitNpc(80601) == npcId then
		if Task_Submit_00080601() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80601
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ä¾¦å¯ŸçŸ³æ¡¥";
		elseif task:HasAcceptedTask(80601) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80601
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ä¾¦å¯ŸçŸ³æ¡¥";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080601_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "ç°åœ¨çš„å½¢åŠ¿ä¸å®¹ä¹è§‚ã€‚";
	action.m_ActionMsg = "æƒ³ä¸åˆ°å¤œæ‘©ç›Ÿçš„çˆªç‰™å·²éå¸ƒå„å¤„ã€‚";
	return action;
end

function Task_00080601_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä¸ºäº†ä¿é™©èµ·è§ï¼Œä½ å†å»ä¾¦æŸ¥ä¸‹çŸ³æ¡¥ï¼Œçœ‹é‚£è¾¹çš„æƒ…å†µå¦‚ä½•ã€‚";
	action.m_ActionMsg = "ä½†æ„¿ä¸ä¼šå¼•æ¥æ›´å¤§çš„å±æœºã€‚";
	return action;
end

function Task_00080601_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æœç„¶åœ¨æˆ‘é¢„æ–™ä¹‹å†…ã€‚	";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080601_step_table = {
		[1] = Task_00080601_step_01,
		[2] = Task_00080601_step_02,
		[10] = Task_00080601_step_10,
		};

function Task_00080601_step(step)
	if Task_00080601_step_table[step] ~= nil then
		return Task_00080601_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080601_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080601() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80601) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00080601_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80601) then
		return false;
	end


	player:AddExp(600000);
	player:getCoin(308000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080601_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80601);
end
