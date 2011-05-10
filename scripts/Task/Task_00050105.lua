--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00050105()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 8 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50105) or task:HasCompletedTask(50105) or task:HasSubmitedTask(50105) then
		return false;
	end
	if not task:HasSubmitedTask(50104) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00050105()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 8 then
		return false;
	end
	if task:HasAcceptedTask(50105) or task:HasCompletedTask(50105) or task:HasSubmitedTask(50105) then
		return false;
	end
	if not task:HasSubmitedTask(50104) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00050105()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50105) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00050105(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50105) == npcId and Task_Accept_00050105 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50105
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ·±è—ä¸éœ²";
	elseif task:GetTaskSubmitNpc(50105) == npcId then
		if Task_Submit_00050105() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50105
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ·±è—ä¸éœ²";
		elseif task:HasAcceptedTask(50105) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50105
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ·±è—ä¸éœ²";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00050105_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "æ–½ä¸»æ‰€ä¸ºä½•äº‹ï¼Ÿ";
	action.m_ActionMsg = "é¦™éŸ³ç‹åå› ä¸ºæƒ³å¿µæ–‡éš£ï¼Œæƒ³æŠŠè¿™ä¸ªå›éŸ³è´æ”¾åœ¨é¾™ç‹åº™ä¸­ã€‚";
	return action;
end

function Task_00050105_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è´«åƒ§äº†è§£ã€‚";
	action.m_ActionMsg = "å¤šè°¢ã€‚ï¼ˆæ‰«åœ°åƒ§ï¼Œè«éæ˜¯æ·±è—ä¸éœ²çš„é«˜æ‰‹ï¼Ÿï¼‰";
	return action;
end

function Task_00050105_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "åä¼šæœ‰æœŸã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050105_step_table = {
		[1] = Task_00050105_step_01,
		[2] = Task_00050105_step_02,
		[10] = Task_00050105_step_10,
		};

function Task_00050105_step(step)
	if Task_00050105_step_table[step] ~= nil then
		return Task_00050105_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00050105_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050105() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50105) then
		return false;
	end
	task:AddTaskStep(50105);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00050105_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8900,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(50105) then
		return false;
	end

	if IsEquipTypeId(8900) then
		for k = 1, 1 do
			package:AddEquip(8900, 1);
		end
	else 
		package:AddItem(8900,1,1);
	end

	player:AddExp(900);
	player:getCoin(700);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00050105_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50105);
end
