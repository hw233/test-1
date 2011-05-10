--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00050103()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 8 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50103) or task:HasCompletedTask(50103) or task:HasSubmitedTask(50103) then
		return false;
	end
	if not task:HasSubmitedTask(50102) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00050103()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 8 then
		return false;
	end
	if task:HasAcceptedTask(50103) or task:HasCompletedTask(50103) or task:HasSubmitedTask(50103) then
		return false;
	end
	if not task:HasSubmitedTask(50102) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00050103()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50103) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00050103(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50103) == npcId and Task_Accept_00050103 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50103
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å›éŸ³è´";
	elseif task:GetTaskSubmitNpc(50103) == npcId then
		if Task_Submit_00050103() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50103
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å›éŸ³è´";
		elseif task:HasAcceptedTask(50103) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50103
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å›éŸ³è´";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00050103_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ—¢ç„¶è¿™æ ·ï¼Œæˆ‘æƒ³ä½ å¯ä»¥å¸®æˆ‘å»åå®å±±çš„è›®èƒ¡å›½ï¼Œæ€æ‰é‚£é‡Œçš„æš´æˆ¾å·¨äººï¼Œæ”¶é›†ä¸€ä¸ªå›éŸ³è´ï¼Œç„¶åäº¤ç»™åäººå ‚çš„å¼ è‰¯ï¼Œä»–åœ¨è¿™æ–¹é¢å¾ˆæ“…é•¿ï¼Œæ€»æ˜¯èƒ½åšå‡ºéå¸¸å¥½å¬çš„æ›²å­ï¼Œè¯·ä»–å½•ä¸€é¦–åœ¨å›éŸ³è´é‡Œï¼Œä»¥åå¯ä»¥æ”¾åœ¨é¾™ç‹åº™é‡Œã€‚";
	action.m_ActionMsg = "æˆ‘æƒ³è¿™ä¼šæ˜¯ä¸€é¦–å¾ˆä¸é”™çš„æ›²å­.";
	return action;
end

function Task_00050103_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‹‡å£«ï¼Œå¾ˆé«˜å…´å†æ¬¡è§åˆ°ä½ ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050103_step_table = {
		[1] = Task_00050103_step_01,
		[10] = Task_00050103_step_10,
		};

function Task_00050103_step(step)
	if Task_00050103_step_table[step] ~= nil then
		return Task_00050103_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00050103_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050103() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50103) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15048,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15048, itemNum, 1);
		end
		task:AddTaskStep2(50103, 1, itemNum);
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00050103_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15048,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(50103) then
		return false;
	end

	package:DelItemAll(15048,1);

	player:AddExp(850);
	player:getCoin(600);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00050103_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15048,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50103);
end
