--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00060803()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 46 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60803) or task:HasCompletedTask(60803) or task:HasSubmitedTask(60803) then
		return false;
	end
	if not task:HasSubmitedTask(60802) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00060803()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 46 then
		return false;
	end
	if task:HasAcceptedTask(60803) or task:HasCompletedTask(60803) or task:HasSubmitedTask(60803) then
		return false;
	end
	if not task:HasSubmitedTask(60802) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00060803()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60803) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00060803(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60803) == npcId and Task_Accept_00060803 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60803
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç¥è¯å¥‡æ•ˆ";
	elseif task:GetTaskSubmitNpc(60803) == npcId then
		if Task_Submit_00060803() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60803
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç¥è¯å¥‡æ•ˆ";
		elseif task:HasAcceptedTask(60803) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60803
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç¥è¯å¥‡æ•ˆ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00060803_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "ï¼Ÿï¼Ÿï¼Ÿ";
	action.m_ActionMsg = "æˆ‘çŸ¥é“ä½ ä¸ä¼šè¯´è¯ï¼Œç¥æŒ‡å¤§å¸ˆè®©ä½ æœä¸‹è¿™æ£µè‰ï¼Œä½ çš„å¤±è¯­ç—‡å°±å¥½äº†ã€‚";
	return action;
end

function Task_00060803_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ï¼ˆæœä¸‹è‰è¯ï¼‰";
	action.m_ActionMsg = "ç°åœ¨æ„Ÿè§‰æ€ä¹ˆæ ·ï¼Ÿ";
	return action;
end

function Task_00060803_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘çš„æ•´ä¸ªå–‰å’™è‚ºè…‘å®Œå…¨é€šäº†ï¼Œç»ˆäºå¯ä»¥è¯´è¯äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060803_step_table = {
		[1] = Task_00060803_step_01,
		[2] = Task_00060803_step_02,
		[10] = Task_00060803_step_10,
		};

function Task_00060803_step(step)
	if Task_00060803_step_table[step] ~= nil then
		return Task_00060803_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00060803_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060803() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60803) then
		return false;
	end
	task:AddTaskStep(60803);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00060803_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8934,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(60803) then
		return false;
	end

	if IsEquipTypeId(8934) then
		for k = 1, 1 do
			package:AddEquip(8934, 1);
		end
	else 
		package:AddItem(8934,1,1);
	end

	player:AddExp(40000);
	player:getCoin(27000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00060803_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60803);
end
