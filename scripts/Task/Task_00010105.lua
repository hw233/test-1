--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00010105()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 5 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10105) or task:HasCompletedTask(10105) or task:HasSubmitedTask(10105) then
		return false;
	end
	if not task:HasSubmitedTask(10104) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00010105()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 5 then
		return false;
	end
	if task:HasAcceptedTask(10105) or task:HasCompletedTask(10105) or task:HasSubmitedTask(10105) then
		return false;
	end
	if not task:HasSubmitedTask(10104) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00010105()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10105) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00010105(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10105) == npcId and Task_Accept_00010105 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10105
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å±æœºå››ä¼";
	elseif task:GetTaskSubmitNpc(10105) == npcId then
		if Task_Submit_00010105() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10105
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å±æœºå››ä¼";
		elseif task:HasAcceptedTask(10105) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10105
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å±æœºå››ä¼";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00010105_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘æœ€è¿‘éå¸¸éœ€è¦äººæ‰‹ï¼Œä½ æ¥çš„æ­£å¥½ï¼Œä¸€ç›´å¬åŸéƒŠçš„æ—æ°‘è¯´æœ€è¿‘å—åˆ°å¾ˆå¤šæ€ªç‰©çš„éªšæ‰°ï¼Œå¯¹ä»–ä»¬çš„ç”Ÿæ´»å½±å“å¾ˆå¤§ï¼Œæˆ‘ç°åœ¨è¦ä½ å»é‚£é‡ŒæŸ¥ä¸ªç©¶ç«Ÿã€‚";
	action.m_ActionMsg = "å±…ç„¶æœ‰æ€ªç‰©æ•¢éªšæ‰°æˆ‘ä»¬çš„æ—æ°‘ï¼Œæˆ‘ä¼šå»æŸ¥çœ‹æ¸…æ¥šçš„ã€‚";
	return action;
end

function Task_00010105_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "åŸæ¥æ˜¯è¿™äº›é‚ªæ¶çš„å®¶ä¼™ï¼Œå¹¸å¥½ä½ æ›¿æˆ‘å‘ç°äº†ä»–ä»¬ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010105_step_table = {
		[1] = Task_00010105_step_01,
		[10] = Task_00010105_step_10,
		};

function Task_00010105_step(step)
	if Task_00010105_step_table[step] ~= nil then
		return Task_00010105_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00010105_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010105() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(10105) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00010105_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(5,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(10105) then
		return false;
	end

	if IsEquipTypeId(5) then
		for k = 1, 1 do
			package:AddEquip(5, 1);
		end
	else 
		package:AddItem(5,1,1);
	end

	player:AddExp(600);
	player:getCoin(700);
	player:getTael(5);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00010105_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10105);
end
