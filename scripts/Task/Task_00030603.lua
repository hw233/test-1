--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00030603()
	local player = GetPlayer();
	if player:GetLev() < 44 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30603) or task:HasCompletedTask(30603) or task:HasSubmitedTask(30603) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(3060201) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(3060202) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00030603()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 44 then
		return false;
	end
	if task:HasAcceptedTask(30603) or task:HasCompletedTask(30603) or task:HasSubmitedTask(30603) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(3060201) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(3060202) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00030603()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30603) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00030603(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30603) == npcId and Task_Accept_00030603 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30603
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å°é¬¼çš„æ„Ÿè°¢";
	elseif task:GetTaskSubmitNpc(30603) == npcId then
		if Task_Submit_00030603() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30603
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å°é¬¼çš„æ„Ÿè°¢";
		elseif task:HasAcceptedTask(30603) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30603
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å°é¬¼çš„æ„Ÿè°¢";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00030603_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "çœŸæ˜¯å¤ªæ„Ÿè°¢æ‚¨äº†ï¼Œæ‚¨çš„å–„å¿ƒæ•‘æ´»äº†æˆ‘çš„å…„å¼Ÿï¼Œæˆ‘ä¸€å®šä¼šè®©æ›´å¤šçš„äººçŸ¥é“ä½ çš„äº‹è¿¹ã€‚";
	action.m_ActionMsg = "è¿™æ˜¯æˆ‘åº”è¯¥åšçš„ï¼Œä¸å¿…å®¢æ°”ã€‚";
	return action;
end

function Task_00030603_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "çœŸæ˜¯ä¸çŸ¥é“è¯¥æ€ä¹ˆæ„Ÿè°¢æ‚¨æ‰å¥½ï¼Œåœ¨å±±æµ·å¤§é™†çš„æ—…é€”è¾›åŠ³ï¼Œæ‚¨ä¸€å®šè¦ç•™ä¸‹æ¥å–æ¯èŒ¶ã€‚";
	action.m_ActionMsg = "æ—¢ç„¶è¿™æ ·ï¼Œåªå¥½ç››æƒ…éš¾å´äº†ã€‚ï¼ˆå°†èŒ¶å–ä¸‹ï¼‰";
	return action;
end

function Task_00030603_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‘µå‘µå“ˆå“ˆï¼Œä¸è¦é˜²å¤‡å¿ƒè¿™ä¹ˆå¼ºå˜›ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030603_step_table = {
		[1] = Task_00030603_step_01,
		[2] = Task_00030603_step_02,
		[10] = Task_00030603_step_10,
		};

function Task_00030603_step(step)
	if Task_00030603_step_table[step] ~= nil then
		return Task_00030603_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00030603_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030603() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30603) then
		return false;
	end
	task:AddTaskStep(30603);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00030603_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8927,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30603) then
		return false;
	end

	if IsEquipTypeId(8927) then
		for k = 1, 1 do
			package:AddEquip(8927, 1);
		end
	else 
		package:AddItem(8927,1,1);
	end

	player:AddExp(32000);
	player:getCoin(22000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00030603_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30603);
end
