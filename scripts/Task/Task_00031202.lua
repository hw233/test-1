--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00031202()
	local player = GetPlayer();
	if player:GetLev() < 59 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31202) or task:HasCompletedTask(31202) or task:HasSubmitedTask(31202) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31201) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31201) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00031202()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 59 then
		return false;
	end
	if task:HasAcceptedTask(31202) or task:HasCompletedTask(31202) or task:HasSubmitedTask(31202) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31201) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31201) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00031202()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31202) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00031202(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31202) == npcId and Task_Accept_00031202 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31202
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ‘©çºçš„ç§˜å¯†";
	elseif task:GetTaskSubmitNpc(31202) == npcId then
		if Task_Submit_00031202() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31202
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ‘©çºçš„ç§˜å¯†";
		elseif task:HasAcceptedTask(31202) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31202
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ‘©çºçš„ç§˜å¯†";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00031202_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "ä½ è¿™ä¹ˆå¿«å°±å›æ¥äº†ï¼Ÿå°†é‚£äº›å¼ºå£®çš„ç–¯ç‹‚é›ªæ€ªæ”¶æœå¹¶ä¸”æ¥å½’é¡ºæˆ‘äº†å—ï¼Ÿ";
	action.m_ActionMsg = "â€¦â€¦æˆ‘å°†ä»–ä»¬å…¨éƒ¨æ€æ­»äº†ã€‚";
	return action;
end

function Task_00031202_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ æ˜¯å‚»å­å—ï¼Ÿæˆ‘æ˜¯è®©ä½ å°†ä»–ä»¬æ”¶æœæ¥å½’é¡ºæˆ‘ï¼Œä»¥ä¾¿å¢å¼ºæˆ‘çš„åŠ¿åŠ›ï¼Œä½ å´å°†ä»–ä»¬æ€æ­»ï¼Œç®€ç›´å‚»åˆ°ä¸è¡Œã€‚";
	action.m_ActionMsg = "ä½ å¦‚æ­¤å±…å¿ƒåµæµ‹ï¼Œå±…ç„¶è¿˜æ€ªæˆ‘ï¼Ÿä½ æ˜¯å¤œæ‘©ç›Ÿçš„å§ã€‚";
	return action;
end

function Task_00031202_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å“¦æˆ‘çš„å¤©å•Šï¼Œæ€ä¹ˆèƒ½è¿™æ ·ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031202_step_table = {
		[1] = Task_00031202_step_01,
		[2] = Task_00031202_step_02,
		[10] = Task_00031202_step_10,
		};

function Task_00031202_step(step)
	if Task_00031202_step_table[step] ~= nil then
		return Task_00031202_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00031202_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031202() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31202) then
		return false;
	end
	task:AddTaskStep(31202);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00031202_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8914,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31202) then
		return false;
	end

	if IsEquipTypeId(8914) then
		for k = 1, 1 do
			package:AddEquip(8914, 1);
		end
	else 
		package:AddItem(8914,1,1);
	end

	player:AddExp(100000);
	player:getCoin(100000);
	player:getTael(30);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00031202_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31202);
end
