--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000019()
	local player = GetPlayer();
	if player:GetLev() < 12 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(19) or task:HasCompletedTask(19) or task:HasSubmitedTask(19) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(18) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(18) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(18) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000019()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 12 then
		return false;
	end
	if task:HasAcceptedTask(19) or task:HasCompletedTask(19) or task:HasSubmitedTask(19) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(18) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(18) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(18) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000019()
	if GetPlayer():GetTaskMgr():HasCompletedTask(19) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000019(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(19) == npcId and Task_Accept_00000019 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 19
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "äº‘çµå±±é™¤å¦–";
	elseif task:GetTaskSubmitNpc(19) == npcId then
		if Task_Submit_00000019() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 19
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "äº‘çµå±±é™¤å¦–";
		elseif task:HasAcceptedTask(19) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 19
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "äº‘çµå±±é™¤å¦–";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000019_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è›‡å¦–ç¡®å®æ¯”è¾ƒå‰å®³ï¼Œä¸è¿‡æˆ‘æ—©å·²å‡†å¤‡äº†åº”ä»˜ä¹‹æ³•ï¼Œç‰¹åœ°ä»é»„å±±çš„é¤éœå¤§å¸ˆé‚£é‡Œå€Ÿæ¥é‡‘é³èœˆèš£ï¼Œå®ƒå¯æ˜¯è›‡å¦–çš„å…‹æ˜Ÿã€‚æ¶ˆç­è¿™åªè›‡å¦–ä¸åœ¨è¯ä¸‹ï¼Œ"..GetPlayerName(GetPlayer()).."ä½ è¿™å°±å’Œæˆ‘ä»¬ä¸€èµ·å»å§ã€‚";
	action.m_ActionMsg = "æ™šè¾ˆæ­£å¥½ä¹Ÿæƒ³å»è§è¯†ä¸€ä¸‹è›‡å¦–çš„å‰å®³ã€‚";
	return action;
end

function Task_00000019_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "åªæ˜¯å¯æƒœè¿™åªé‡‘é³èœˆèš£äº†ï¼Œç«Ÿç„¶å’Œè›‡å¦–åŒå½’äºå°½ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000019_step_table = {
		[1] = Task_00000019_step_01,
		[10] = Task_00000019_step_10,
		};

function Task_00000019_step(step)
	if Task_00000019_step_table[step] ~= nil then
		return Task_00000019_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000019_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000019() then
		return false;
	end
	if not task:AcceptTask(19) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000019_submit(itemId, itemNum)
	local player = GetPlayer();

	--¼ì²éÑ¡ÔñĞÔÎïÆ·
	local select = false;
	if itemId == 1600 and itemNum == 1 then
		select = true;
	elseif itemId == 1601 and itemNum == 1 then
		select = true;
	elseif itemId == 1602 and itemNum == 1 then
		select = true;
	end

	if not select then return false; end
	local package = player:GetPackage();

	local selReqGrid = package:GetItemUsedGrids(itemId, itemNum, 1);
	if selReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 1013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(19) then
		return false;
	end

	if IsEquipTypeId(itemId) then 
		for j = 1, itemNum do
			package:AddEquip(itemId, 1);
		end
	else
		package:AddItem(itemId, itemNum, 1);
	end

	player:AddExp(4400);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000019_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(19);
end
