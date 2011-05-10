--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00010305()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 18 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10305) or task:HasCompletedTask(10305) or task:HasSubmitedTask(10305) then
		return false;
	end
	if not task:HasSubmitedTask(10304) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00010305()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 18 then
		return false;
	end
	if task:HasAcceptedTask(10305) or task:HasCompletedTask(10305) or task:HasSubmitedTask(10305) then
		return false;
	end
	if not task:HasSubmitedTask(10304) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00010305()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10305) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00010305(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10305) == npcId and Task_Accept_00010305 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10305
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ä¼ é€’å¯†ä»¤";
	elseif task:GetTaskSubmitNpc(10305) == npcId then
		if Task_Submit_00010305() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10305
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ä¼ é€’å¯†ä»¤";
		elseif task:HasAcceptedTask(10305) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10305
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ä¼ é€’å¯†ä»¤";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00010305_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "ç°åœ¨æˆ‘è¦ä½ å¸®æˆ‘ç§˜å¯†ä¼ ä»¤ä¸‹å»ï¼Œå‘Šè¯‰åœ¨å–„æ³•å ‚çš„æŠ¤æ³•å†›å¸ˆï¼Œå³æ—¥èµ·å¼€å§‹å‡å°‘ç»™é­”æ—ç”˜éœ²çš„ä¾›åº”ï¼Œåˆ°ä¸‹ä¸ªæœˆå°±å®Œå…¨åœæ­¢ä¾›åº”ã€‚";
	action.m_ActionMsg = "æˆ‘è¿™å°±å¸®æ‚¨ä¼ ä»¤ä¸‹å»ã€‚";
	return action;
end

function Task_00010305_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä¸èƒ½å†è¿™æ ·è¿å°±é­”æ—çš„è´ªå©ªäº†ï¼Œä½†æ˜¯æ‚¦æ„ç‹å¦ƒé‚£é‡Œï¼Œæœ€å¥½ä¸è¦è®©å¥¹çŸ¥é“è¿™ä»¶äº‹æƒ…ã€‚";
	action.m_ActionMsg = "æ‚¨çš„è€ƒè™‘å¾ˆæœ‰é“ç†ï¼Œæˆ‘ä¼šéµç…§æ‚¨çš„å©å’åšçš„ã€‚";
	return action;
end

function Task_00010305_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ¥ä»¤ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010305_step_table = {
		[1] = Task_00010305_step_01,
		[2] = Task_00010305_step_02,
		[10] = Task_00010305_step_10,
		};

function Task_00010305_step(step)
	if Task_00010305_step_table[step] ~= nil then
		return Task_00010305_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00010305_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010305() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15023, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(10305) then
		return false;
	end
	task:AddTaskStep(10305);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00010305_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15023,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8925,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(10305) then
		return false;
	end

	if IsEquipTypeId(8925) then
		for k = 1, 1 do
			package:AddEquip(8925, 1);
		end
	else 
		package:AddItem(8925,1,1);
	end
	package:DelItemAll(15023,1);

	player:AddExp(1800);
	player:getCoin(2300);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00010305_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15023,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10305);
end
