--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00040504()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40504) or task:HasCompletedTask(40504) or task:HasSubmitedTask(40504) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00040504()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(40504) or task:HasCompletedTask(40504) or task:HasSubmitedTask(40504) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00040504()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40504) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00040504(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40504) == npcId and Task_Accept_00040504 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40504
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç‹çš„å¥–èµ";
	elseif task:GetTaskSubmitNpc(40504) == npcId then
		if Task_Submit_00040504() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40504
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç‹çš„å¥–èµ";
		elseif task:HasAcceptedTask(40504) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40504
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç‹çš„å¥–èµ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00040504_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "æ©ï¼Œä½ æœ€è¿‘è¿›æ­¥å¾ˆå¿«ï¼Œæˆ‘å¾ˆé«˜å…´ï¼Œä¸æ„§æ˜¯æˆ‘ä»¬å¤©æ—çš„å‹‡å£«ã€‚";
	action.m_ActionMsg = "æ‚¨è¿‡å¥–äº†ï¼Œæˆ‘ä¼šåŠªåŠ›è®­ç»ƒè‡ªå·±ï¼Œäº‰å–æ—©æ—¥ä¸ºå¤©æ—æ•ˆåŠ›ã€‚";
	return action;
end

function Task_00040504_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘è®©é“åŒ é“ºçš„å¼ºåŒ–å¤§å¸ˆç»™ä½ åšäº†ä¸€æŠŠå®å‰‘ï¼Œä½œä¸ºå¥–èµèµäºˆä½ ï¼Œç°åœ¨ä½ å°±å»æ‹¿å§ï¼Œè¿™æŠŠå‰‘ä¼šä½¿ä½ æˆ˜æ— ä¸èƒœã€‚";
	action.m_ActionMsg = "è°¢è°¢æ‚¨çš„èµèµï¼Œæˆ‘è¿™å°±å»ã€‚";
	return action;
end

function Task_00040504_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‹‡å£«ï¼Œæˆ‘ç­‰ä½ å¾ˆä¹…äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040504_step_table = {
		[1] = Task_00040504_step_01,
		[2] = Task_00040504_step_02,
		[10] = Task_00040504_step_10,
		};

function Task_00040504_step(step)
	if Task_00040504_step_table[step] ~= nil then
		return Task_00040504_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00040504_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040504() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40504) then
		return false;
	end
	task:AddTaskStep(40504);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00040504_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(368,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(40504) then
		return false;
	end

	if IsEquipTypeId(368) then
		for k = 1, 1 do
			package:AddEquip(368, 1);
		end
	else 
		package:AddItem(368,1,1);
	end

	player:AddExp(2000);
	player:getCoin(10000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00040504_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40504);
end
