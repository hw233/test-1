--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000030()
	local player = GetPlayer();
	if player:GetLev() < 11 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30) or task:HasCompletedTask(30) or task:HasSubmitedTask(30) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(29) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(29) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(29) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000030()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 11 then
		return false;
	end
	if task:HasAcceptedTask(30) or task:HasCompletedTask(30) or task:HasSubmitedTask(30) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(29) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(29) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(29) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000030()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000030(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30) == npcId and Task_Accept_00000030 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å†ä¼šé†‰é“äºº";
	elseif task:GetTaskSubmitNpc(30) == npcId then
		if Task_Submit_00000030() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å†ä¼šé†‰é“äºº";
		elseif task:HasAcceptedTask(30) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å†ä¼šé†‰é“äºº";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000030_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è™½ç„¶æ…ˆäº‘å¯ºå¤§æˆ˜åœ¨å³ï¼Œä¸è¿‡æ­£é‚ªåŒæ–¹éƒ½åœ¨é‚€è¯·å¸®æ‰‹ï¼Œæ‰€ä»¥æš‚æ—¶æ˜¯é£å¹³æµªé™çš„ï¼Œä½ å…ˆå»æˆéƒ½æœ›æ±Ÿæ¥¼å§ï¼Œé†‰é“äººå¸ˆå…„ä»–ä¸€èˆ¬éƒ½åœ¨é‚£é‡Œé¥®é…’ã€‚";
	action.m_ActionMsg = "å¤šè°¢å¤§å¸ˆèµ é€çš„æ³•å®ï¼Œæˆ‘è¿™å°±å»äº†ã€‚";
	return action;
end

function Task_00000030_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ å°å­è¿æ°”ä¸é”™å˜›ï¼Œç™½äº‘é‚£å°å§‘å¨˜ç«Ÿç„¶æŠŠå¥¹çš„éšèº«è‡³å®éƒ½é€ä½ äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000030_step_table = {
		[1] = Task_00000030_step_01,
		[10] = Task_00000030_step_10,
		};

function Task_00000030_step(step)
	if Task_00000030_step_table[step] ~= nil then
		return Task_00000030_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000030_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000030() then
		return false;
	end
	if not task:AcceptTask(30) then
		return false;
	end
	task:AddTaskStep(30);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000030_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(502,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30) then
		return false;
	end

	if IsEquipTypeId(502) then
		for k = 1, 1 do
			package:AddEquip(502, 1);
		end
	else 
		package:AddItem(502,1,1);
	end

	player:AddExp(2300);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000030_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(30);
end
