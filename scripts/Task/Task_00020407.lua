--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00020407()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 22 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20407) or task:HasCompletedTask(20407) or task:HasSubmitedTask(20407) then
		return false;
	end
	if not task:HasSubmitedTask(20406) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00020407()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 22 then
		return false;
	end
	if task:HasAcceptedTask(20407) or task:HasCompletedTask(20407) or task:HasSubmitedTask(20407) then
		return false;
	end
	if not task:HasSubmitedTask(20406) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00020407()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20407) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00020407(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20407) == npcId and Task_Accept_00020407 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20407
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "åˆºå®¢çš„ç§˜å¯†";
	elseif task:GetTaskSubmitNpc(20407) == npcId then
		if Task_Submit_00020407() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20407
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "åˆºå®¢çš„ç§˜å¯†";
		elseif task:HasAcceptedTask(20407) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20407
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "åˆºå®¢çš„ç§˜å¯†";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00020407_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "ä½ æ¥è¿™é‡Œåšä»€ä¹ˆï¼Ÿå°±å‡­ä½ ä¸€ä¸ªå°å°çš„é­”æ—å‹‡å£«ï¼Œå‡­ä»€ä¹ˆè®©æˆ‘è¯´æˆ‘å°±è¯´ï¼Œå†µä¸”æˆ‘æ ¹æœ¬å°±ä»€ä¹ˆéƒ½ä¸çŸ¥é“ï¼Œä½ é—®æˆ‘ä¹Ÿæ²¡æœ‰ç”¨ã€‚";
	action.m_ActionMsg = "å¸é‡Šå¤©ä¸€å®šæ˜¯æœ‰ä»€ä¹ˆé˜´è°‹ï¼Œä¸ç„¶ä¸ä¼šè¿™æ ·ã€‚";
	return action;
end

function Task_00020407_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä¸ç®¡æ€æ ·ï¼Œä½ ä»æˆ‘è¿™é‡Œæ˜¯é—®ä¸å‡ºä»€ä¹ˆæ¥çš„ï¼Œæœ‰æœ¬äº‹ä½ å°±å»æ‰“è´¥å…¶ä»–çš„å¤©æ—åˆºå®¢ï¼Œçœ‹ä»–ä»¬è‚¯ä¸è‚¯è¯´äº†ã€‚\n";
	action.m_ActionMsg = "å¥½çš„ï¼Œä½ ç­‰ç€ç§ã€‚";
	return action;
end

function Task_00020407_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘æƒ³ä½ ä¸€å®šæ˜¯äº†è§£åˆ°æƒ…å†µäº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020407_step_table = {
		[1] = Task_00020407_step_01,
		[2] = Task_00020407_step_02,
		[10] = Task_00020407_step_10,
		};

function Task_00020407_step(step)
	if Task_00020407_step_table[step] ~= nil then
		return Task_00020407_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00020407_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020407() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20407) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15030,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15030, itemNum, 1);
		end
		task:AddTaskStep2(20407, 1, itemNum);
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00020407_submit(itemId, itemNum)
	local player = GetPlayer();

	--¼ì²éÑ¡ÔñĞÔÎïÆ·
	local select = false;
	if itemId == 130 and itemNum == 1 then
		select = true;
	elseif itemId == 132 and itemNum == 1 then
		select = true;
	elseif itemId == 134 and itemNum == 1 then
		select = true;
	end

	if not select then return false; end
	local package = player:GetPackage();
	if package:GetItemNum(15030,1) < 1 then
		return false;
	end

	local selReqGrid = package:GetItemUsedGrids(itemId, itemNum, 1);
	if selReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(20407) then
		return false;
	end

	if IsEquipTypeId(itemId) then 
		for j = 1, itemNum do
			package:AddEquip(itemId, 1);
		end
	else
		package:AddItem(itemId, itemNum, 1);
	end
	package:DelItemAll(15030,1);

	player:AddExp(2200);
	player:getCoin(4100);
	player:getTael(10);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00020407_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15030,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20407);
end
