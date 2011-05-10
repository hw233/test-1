--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00060404()
	local player = GetPlayer();
	if player:GetLev() < 36 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60404) or task:HasCompletedTask(60404) or task:HasSubmitedTask(60404) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60403) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60403) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00060404()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 36 then
		return false;
	end
	if task:HasAcceptedTask(60404) or task:HasCompletedTask(60404) or task:HasSubmitedTask(60404) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60403) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60403) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00060404()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60404) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00060404(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60404) == npcId and Task_Accept_00060404 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60404
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å®‰é­‚ä»ªå¼";
	elseif task:GetTaskSubmitNpc(60404) == npcId then
		if Task_Submit_00060404() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60404
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å®‰é­‚ä»ªå¼";
		elseif task:HasAcceptedTask(60404) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60404
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å®‰é­‚ä»ªå¼";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00060404_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "åœ¨ä½ å»æ¸…é™¤é‚£äº›æ€ªç‰©çš„æ—¶å€™ï¼Œæˆ‘åˆ¶ä½œäº†ä¸€ä¸ªå®‰é­‚æ³•é˜µï¼Œä½ åªè¦åœ¨è¿™äº›äº¡é­‚æ›¾ç»æˆ˜æ–—å’Œç‰ºç‰²çš„åœ°æ–¹é˜¿å…°è‹¥å¤æˆ˜åœºä½¿ç”¨è¿™ä¸ªå®‰é­‚æ³•é˜µï¼Œä¾¿å¯ä»¥ä½¿ä»–ä»¬çš„æ¸¸è¡çš„çµé­‚å½’äºå¹³é™ã€‚";
	action.m_ActionMsg = "æ˜ç™½äº†ï¼Œé‚£æˆ‘ç°åœ¨å»é˜¿å…°è‹¥å¤æˆ˜åœºäº†ã€‚";
	return action;
end

function Task_00060404_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ‰€æœ‰çš„çµé­‚ä»¿ä½›éƒ½åœ¨è¯´ï¼Œæ„Ÿè°¢ä½ å‹‡å£«ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060404_step_table = {
		[1] = Task_00060404_step_01,
		[10] = Task_00060404_step_10,
		};

function Task_00060404_step(step)
	if Task_00060404_step_table[step] ~= nil then
		return Task_00060404_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00060404_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060404() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15005, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(60404) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00060404_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15005,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8918,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(60404) then
		return false;
	end

	if IsEquipTypeId(8918) then
		for k = 1, 1 do
			package:AddEquip(8918, 1);
		end
	else 
		package:AddItem(8918,1,1);
	end
	package:DelItemAll(15005,1);

	player:AddExp(5500);
	player:getCoin(8000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00060404_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15005,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60404);
end
