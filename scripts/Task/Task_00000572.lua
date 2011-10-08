--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000572()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(572) or task:HasCompletedTask(572) or task:HasSubmitedTask(572) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000572()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(572) or task:HasCompletedTask(572) or task:HasSubmitedTask(572) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000572()
	if GetPlayer():GetTaskMgr():HasCompletedTask(572) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000572(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(572) == npcId and Task_Accept_00000572 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 572
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "é‚ªæ¶é“äºº";
	elseif task:GetTaskSubmitNpc(572) == npcId then
		if Task_Submit_00000572() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 572
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "é‚ªæ¶é“äºº";
		elseif task:HasAcceptedTask(572) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 572
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
function Task_00000572_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ¡‚èŠ±å±±æœ‰ä¸€åº§æé˜´æ´ï¼Œå¬ä¹¡æ°‘ä»¬è¯´é‚£é‡Œä¸çŸ¥ä»€ä¹ˆæ—¶å€™æ¥äº†ä¸€ä¸ªé‚ªæ¶çš„é“äººï¼Œå››å¤„æ³æ å¹´é’çš„å°‘ç”·ç”·å¥³å›å»ä¿®ç‚¼é‚ªåŠŸï¼Œä½ é€Ÿé€Ÿå»å°†ä»–é™¤æ‰ã€‚";
	action.m_ActionMsg = "å¼Ÿå­éµå‘½ï¼Œæˆ‘å»å»å°±å›ã€‚";
	return action;
end

function Task_00000572_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ çœŸæ˜¯èº«æ‰‹ä¸å‡¡å•Šã€‚ ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000572_step_table = {
		[1] = Task_00000572_step_01,
		[10] = Task_00000572_step_10,
		};

function Task_00000572_step(step)
	if Task_00000572_step_table[step] ~= nil then
		return Task_00000572_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000572_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000572() then
		return false;
	end
	if not task:AcceptTask(572) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000572_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(572) then
		return false;
	end


	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000572_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(572);
end
