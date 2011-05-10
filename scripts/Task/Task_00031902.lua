--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00031902()
	local player = GetPlayer();
	if player:GetLev() < 70 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31902) or task:HasCompletedTask(31902) or task:HasSubmitedTask(31902) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31901) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31901) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00031902()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 70 then
		return false;
	end
	if task:HasAcceptedTask(31902) or task:HasCompletedTask(31902) or task:HasSubmitedTask(31902) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31901) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31901) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00031902()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31902) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00031902(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31902) == npcId and Task_Accept_00031902 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31902
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "è¿½æ€å•å¸ƒ";
	elseif task:GetTaskSubmitNpc(31902) == npcId then
		if Task_Submit_00031902() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31902
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "è¿½æ€å•å¸ƒ";
		elseif task:HasAcceptedTask(31902) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31902
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "è¿½æ€å•å¸ƒ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00031902_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä¸è¿‡å°½ç®¡è¿™æ ·ï¼Œæˆ‘è¿˜æ˜¯ç›¸ä¿¡ä¸Šå¤©æ³¨å®šäº†æˆ‘ä¸å•å¸ƒçš„ç¼˜åˆ†æœªå°½ï¼Œæ—¢ç„¶æˆ‘èƒ½è¿½åˆ°å±±æµ·å¤§é™†æ¥ï¼Œæˆ‘å°±ä¸€å®šè¦æƒ³åŠæ³•å†è§åˆ°ä»–ï¼Œè¿™ä¸ªè¿‡ç¨‹æœ‰ç‚¹å¤æ‚ï¼Œè€Œä¸”æˆ‘éœ€è¦å¸®å¿™ï¼Œä½ æ„¿æ„å¸®åŠ©æˆ‘å—ï¼Ÿ";
	action.m_ActionMsg = "æˆ‘ä¸€å®šä¼šå¸®å¿™çš„ã€‚";
	return action;
end

function Task_00031902_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ—¢ç„¶ä½ æ„¿æ„å¸®å¿™ï¼Œé‚£æˆ‘å°±ä¸è¿½ç©¶äº†ï¼Œå¸Œæœ›ä½ ç”¨å¿ƒçš„å¸®æˆ‘ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031902_step_table = {
		[1] = Task_00031902_step_01,
		[10] = Task_00031902_step_10,
		};

function Task_00031902_step(step)
	if Task_00031902_step_table[step] ~= nil then
		return Task_00031902_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00031902_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031902() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31902) then
		return false;
	end
	task:AddTaskStep(31902);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00031902_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8915,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31902) then
		return false;
	end

	if IsEquipTypeId(8915) then
		for k = 1, 1 do
			package:AddEquip(8915, 1);
		end
	else 
		package:AddItem(8915,1,1);
	end

	player:AddExp(230000);
	player:getCoin(217500);
	player:getTael(40);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00031902_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31902);
end
