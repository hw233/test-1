--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000001()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(1) or task:HasCompletedTask(1) or task:HasSubmitedTask(1) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(123) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(123) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(123) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000001()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(1) or task:HasCompletedTask(1) or task:HasSubmitedTask(1) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(123) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(123) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(123) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000001()
	if GetPlayer():GetTaskMgr():HasCompletedTask(1) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000001(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(1) == npcId and Task_Accept_00000001 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 1
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "åº”åŠ«ä¹‹äºº";
	elseif task:GetTaskSubmitNpc(1) == npcId then
		if Task_Submit_00000001() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 1
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "åº”åŠ«ä¹‹äºº";
		elseif task:HasAcceptedTask(1) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 1
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "åº”åŠ«ä¹‹äºº";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000001_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç›®å‰èœ€ä¸­ä¹‹åœ°æ­£æ˜¯æ­£é‚ªä¹‹æˆ˜çš„å…³é”®ä¹‹åœ°ã€‚æˆ‘å·²æ¨ç®—å‡ºæ•°ä¸ªåº”åŠ«ä¹‹äººï¼Œå…¶ä¸­æœ‰ä¸€ä½å°±åœ¨ä¸è¿œçš„ä¹Œé¸¦å˜´æ¸¡å£ï¼Œä½ ç°åœ¨é€Ÿé€Ÿå»æ¹˜æ±Ÿè¾¹æ¶ˆç­é‚£äº›è¿½æ€æå®çˆ¶å¥³çš„æ°´è´¼ï¼Œä¿æŠ¤å¥½ä»–ä»¬çˆ¶å¥³ã€‚";
	action.m_ActionMsg = "å¥½ï¼Œæˆ‘è¿™å°±å»ã€‚";
	return action;
end

function Task_00000001_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¤šè°¢å°‘ä¾ ä½ å¸®æˆ‘ä»¬å‡»é€€è¿™äº›æ¶äººï¼Œè¿™æŸ„å‰‘æ˜¯è€å¤«æ˜”å¹´è¡Œèµ°æ±Ÿæ¹–çš„éšèº«å…µåˆƒå°±èµ ä¸å°‘ä¾ å§ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000001_step_table = {
		[1] = Task_00000001_step_01,
		[10] = Task_00000001_step_10,
		};

function Task_00000001_step(step)
	if Task_00000001_step_table[step] ~= nil then
		return Task_00000001_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000001_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000001() then
		return false;
	end
	if not task:AcceptTask(1) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000001_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(2000,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 1013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(1) then
		return false;
	end

	if IsEquipTypeId(2000) then
		for k = 1, 1 do
			package:AddEquip(2000, 1);
		end
	else 
		package:AddItem(2000,1,1);
	end

	player:AddExp(460);
	player:getTael(200);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000001_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(1);
end
