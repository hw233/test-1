--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000003()
	local player = GetPlayer();
	if player:GetLev() < 3 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(3) or task:HasCompletedTask(3) or task:HasSubmitedTask(3) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(2) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(2) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(2) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000003()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 3 then
		return false;
	end
	if task:HasAcceptedTask(3) or task:HasCompletedTask(3) or task:HasSubmitedTask(3) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(2) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(2) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(2) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000003()
	if GetPlayer():GetTaskMgr():HasCompletedTask(3) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000003(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(3) == npcId and Task_Accept_00000003 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 3
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å‘¨æ·³è’™é¦†";
	elseif task:GetTaskSubmitNpc(3) == npcId then
		if Task_Submit_00000003() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 3
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å‘¨æ·³è’™é¦†";
		elseif task:HasAcceptedTask(3) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 3
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å‘¨æ·³è’™é¦†";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000003_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘å…ˆå‰åœ¨ä¹Œé¸¦å˜´å¶é‡äºŒå¼Ÿå‘¨æ·³ï¼Œçº¦å¥½åœ¨ä»–æ•™æˆå­¦ç”Ÿçš„è’™é¦†ç›¸è§ï¼Œè°çŸ¥åˆšåˆ†æ‰‹å°±é‡åˆ°æ˜”å¹´çš„ä»‡å®¶ï¼Œè¿˜å¥½å°‘ä¾ åŠæ—¶èµ¶åˆ°ã€‚åªæ˜¯ç°åœ¨æˆ‘å’Œå°å¥³éƒ½æœ‰ä¼¤åœ¨èº«ï¼Œéœ€è°ƒå…»ç–—ä¼¤ï¼Œæœ›å°‘ä¾ å…ˆå»å‘ŠçŸ¥å‘¨æ·³ï¼Œæˆ‘ä»¬æ­‡æ¯ç‰‡åˆ»å³æ¥ã€‚";
	action.m_ActionMsg = "å¥½çš„ï¼Œæˆ‘ä¼šå‘ŠçŸ¥å‘¨å‰è¾ˆçš„ã€‚";
	return action;
end

function Task_00000003_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æå®å—ä¼¤äº†ï¼Ÿï¼å¤šè°¢å°‘ä¾ ç›¸æ•‘ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000003_step_table = {
		[1] = Task_00000003_step_01,
		[10] = Task_00000003_step_10,
		};

function Task_00000003_step(step)
	if Task_00000003_step_table[step] ~= nil then
		return Task_00000003_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000003_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000003() then
		return false;
	end
	if not task:AcceptTask(3) then
		return false;
	end
	task:AddTaskStep(3);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000003_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(2005,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 1013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(3) then
		return false;
	end

	if IsEquipTypeId(2005) then
		for k = 1, 1 do
			package:AddEquip(2005, 1);
		end
	else 
		package:AddItem(2005,1,1);
	end

	player:AddExp(530);
	player:getTael(200);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000003_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(3);
end
