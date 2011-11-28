--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000617()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(617) or task:HasCompletedTask(617) or task:HasSubmitedTask(617) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000617()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(617) or task:HasCompletedTask(617) or task:HasSubmitedTask(617) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000617()
	if GetPlayer():GetTaskMgr():HasCompletedTask(617) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000617(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(617) == npcId and Task_Accept_00000617 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 617
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "é‚ªæ¶é“äºº";
	elseif task:GetTaskSubmitNpc(617) == npcId then
		if Task_Submit_00000617() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 617
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "é‚ªæ¶é“äºº";
		elseif task:HasAcceptedTask(617) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 617
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "é‚ªæ¶é“äºº";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000617_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ¡‚èŠ±å±±æœ‰ä¸€åº§æé˜´æ´ï¼Œå¬ä¹¡æ°‘ä»¬è¯´é‚£é‡Œä¸çŸ¥ä»€ä¹ˆæ—¶å€™æ¥äº†ä¸€ä¸ªé‚ªæ¶çš„é“äººï¼Œå››å¤„æ³æ å¹´é’çš„å°‘ç”·ç”·å¥³å›å»ä¿®ç‚¼é‚ªåŠŸï¼Œä½ é€Ÿé€Ÿå»å°†ä»–é™¤æ‰ã€‚";
	action.m_ActionMsg = "å¥½çš„ï¼Œé™¤æ¶æ‰¬å–„ä¹ƒæ˜¯æˆ‘ä»¬æ­£é“æœ¬è‰²ã€‚";
	return action;
end

function Task_00000617_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ æœç„¶æ˜¯èº«æ‰‹ä¸å‡¡ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000617_step_table = {
		[1] = Task_00000617_step_01,
		[10] = Task_00000617_step_10,
		};

function Task_00000617_step(step)
	if Task_00000617_step_table[step] ~= nil then
		return Task_00000617_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000617_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000617() then
		return false;
	end
	if not task:AcceptTask(617) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000617_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(617) then
		return false;
	end


	player:AddExp(3000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000617_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(617);
end
