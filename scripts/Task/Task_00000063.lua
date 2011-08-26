--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000063()
	local player = GetPlayer();
	if player:GetLev() < 35 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(63) or task:HasCompletedTask(63) or task:HasSubmitedTask(63) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(62) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000063()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 35 then
		return false;
	end
	if task:HasAcceptedTask(63) or task:HasCompletedTask(63) or task:HasSubmitedTask(63) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(62) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000063()
	if GetPlayer():GetTaskMgr():HasCompletedTask(63) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000063(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(63) == npcId and Task_Accept_00000063 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 63
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ä¿®ç‚¼é‚ªåŠŸ";
	elseif task:GetTaskSubmitNpc(63) == npcId then
		if Task_Submit_00000063() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 63
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ä¿®ç‚¼é‚ªåŠŸ";
		elseif task:HasAcceptedTask(63) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 63
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ä¿®ç‚¼é‚ªåŠŸ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000063_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™ä¸ªé¬¼é“äººå¥¸çŒ¾å¾—å¾ˆï¼Œè§æˆ‘æ¥äº†å¸®æ‰‹å°±é€ƒè¿›äº†åæ´ä¸¹å®¤ï¼Œä»–åŠ«æ äº†ä¸å°‘çš„å°‘å¹´ç”·å¥³åœ¨æé˜´æ´é‡Œä¿®ç‚¼é‚ªåŠŸï¼Œå°‘ä¾ å¯ä¸€å®šä¸è¦æ”¾è¿‡ä»–ã€‚";
	action.m_ActionMsg = "çœŸæ˜¯èªæ˜çš„å¤§çŒ©çŒ©ã€‚";
	return action;
end

function Task_00000063_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°æ ·ï¼Œçœ‹ä½ å¾—ç‘Ÿçš„ï¼Œä¼šç©é£å‰‘äº†ä¸èµ·å•Šâ€¦â€¦ç­‰ä½ æ‹¿åˆ°ç´«é’åŒå‰‘å†æ¥æ˜¾æ‘†å§ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000063_step_table = {
		[1] = Task_00000063_step_01,
		[10] = Task_00000063_step_10,
		};

function Task_00000063_step(step)
	if Task_00000063_step_table[step] ~= nil then
		return Task_00000063_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000063_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000063() then
		return false;
	end
	if not task:AcceptTask(63) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000063_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(63) then
		return false;
	end


	player:AddExp(18000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000063_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(63);
end
