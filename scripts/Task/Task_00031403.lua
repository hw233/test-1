--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00031403()
	local player = GetPlayer();
	if player:GetLev() < 62 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31403) or task:HasCompletedTask(31403) or task:HasSubmitedTask(31403) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31402) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31402) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00031403()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 62 then
		return false;
	end
	if task:HasAcceptedTask(31403) or task:HasCompletedTask(31403) or task:HasSubmitedTask(31403) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31402) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31402) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00031403()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31403) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00031403(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31403) == npcId and Task_Accept_00031403 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31403
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ¯’æ°”å¼¥æ¼«";
	elseif task:GetTaskSubmitNpc(31403) == npcId then
		if Task_Submit_00031403() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31403
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ¯’æ°”å¼¥æ¼«";
		elseif task:HasAcceptedTask(31403) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31403
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ¯’æ°”å¼¥æ¼«";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00031403_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‹‡å£«è¯·ä½ ç°åœ¨é©¬ä¸Šå†è¿‡å»ä¸€è¶Ÿï¼Œåˆšåˆšçš„æ¯’æ°”åªèƒ½ç»´æŒç‹¼æ—å®ˆå«5åˆ†é’Ÿçš„æ˜ç¡æ—¶é—´ï¼Œä¸€å®šè¦åœ¨è¿™ä¹‹å‰å°†ç¢å¿ƒå¡”çš„é’¥åŒ™å·å‡ºæ¥ï¼Œä¸ç„¶ä»–ä»¬å°±ä¼šé†’è¿‡æ¥ã€‚";
	action.m_ActionMsg = "é‚£æˆ‘ç°åœ¨å°±è¿‡å»ã€‚";
	return action;
end

function Task_00031403_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‹‡å£«ä½ å¤ªè‹±å‹‡äº†ï¼Œå±…ç„¶å¸®æˆ‘ä»¬å¼„åˆ°äº†ç¢å¿ƒå¡”çš„é’¥åŒ™ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031403_step_table = {
		[1] = Task_00031403_step_01,
		[10] = Task_00031403_step_10,
		};

function Task_00031403_step(step)
	if Task_00031403_step_table[step] ~= nil then
		return Task_00031403_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00031403_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031403() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31403) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15073,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15073, itemNum, 1);
		end
		task:AddTaskStep2(31403, 1, itemNum);
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00031403_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15073,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8912,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31403) then
		return false;
	end

	if IsEquipTypeId(8912) then
		for k = 1, 1 do
			package:AddEquip(8912, 1);
		end
	else 
		package:AddItem(8912,1,1);
	end
	package:DelItemAll(15073,1);

	player:AddExp(150000);
	player:getCoin(125000);
	player:getTael(30);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00031403_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15073,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31403);
end
