--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00062205()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(62205) or task:HasCompletedTask(62205) or task:HasSubmitedTask(62205) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62204) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62204) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00062205()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(62205) or task:HasCompletedTask(62205) or task:HasSubmitedTask(62205) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62204) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62204) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00062205()
	if GetPlayer():GetTaskMgr():HasCompletedTask(62205) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00062205(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(62205) == npcId and Task_Accept_00062205 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 62205
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ¯ç­ç„šéª¨çªŸ";
	elseif task:GetTaskSubmitNpc(62205) == npcId then
		if Task_Submit_00062205() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62205
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ¯ç­ç„šéª¨çªŸ";
		elseif task:HasAcceptedTask(62205) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62205
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ¯ç­ç„šéª¨çªŸ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00062205_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "æˆ‘ååˆ†ä½©æœä½ çš„å‹‡æ°”ï¼";
	action.m_ActionMsg = "æˆ‘å¯æ˜¯é¡¶å¤©ç«‹åœ°çš„è‹±é›„ã€‚";
	return action;
end

function Task_00062205_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‹‡å£«ï¼Œæ®è¯´é‚£é‡Œä¸€å…±æœ‰15å±‚ï¼Œç¬¬15å±‚çš„é‚£ä¸ªå¤§å®¶ä¼™å¯ä¸æ˜¯å¥½å¯¹ä»˜çš„ï¼Œå¦‚æœä½ çœŸçš„æ˜¯è‹±é›„ï¼Œå°±å»æ¶ˆç­ä»–ä»¬å§ã€‚";
	action.m_ActionMsg = "æ”¾å¿ƒï¼Œå¦‚æœä½ çš„å…„å¼Ÿè¿˜æ´»ç€ï¼Œæˆ‘ä¼šå¸¦ä»–ä»¬å›æ¥ã€‚	\n";
	return action;
end

function Task_00062205_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ æ˜¯åå‰¯å…¶å®çš„è‹±é›„ï¼Œè®©åœ¨ä¸‹ä½©æœã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00062205_step_table = {
		[1] = Task_00062205_step_01,
		[2] = Task_00062205_step_02,
		[10] = Task_00062205_step_10,
		};

function Task_00062205_step(step)
	if Task_00062205_step_table[step] ~= nil then
		return Task_00062205_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00062205_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00062205() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(62205) then
		return false;
	end
	if player:isDungeonPassed(1, 0) then
		task:AddTaskStep4(62205, 1, 15);
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00062205_submit(itemId, itemNum)
	local player = GetPlayer();

	--¼ì²éÑ¡ÔñĞÔÎïÆ·
	local select = false;
	if itemId == 1406 and itemNum == 1 then
		select = true;
	elseif itemId == 1401 and itemNum == 1 then
		select = true;
	elseif itemId == 1403 and itemNum == 1 then
		select = true;
	end

	if not select then return false; end
	local package = player:GetPackage();

	local selReqGrid = package:GetItemUsedGrids(itemId, itemNum, 1);
	if selReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(62205) then
		return false;
	end

	if IsEquipTypeId(itemId) then 
		for j = 1, itemNum do
			package:AddEquip(itemId, 1);
		end
	else
		package:AddItem(itemId, itemNum, 1);
	end

	player:AddExp(10000);
	player:getCoin(20000);
	player:getTael(200);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00062205_abandon()
	return GetPlayer():GetTaskMgr():AbandonTask(62205);
end
