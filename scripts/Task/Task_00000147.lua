--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000147()
	local player = GetPlayer();
	if player:GetLev() < 60 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(147) or task:HasCompletedTask(147) or task:HasSubmitedTask(147) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000147()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 60 then
		return false;
	end
	if task:HasAcceptedTask(147) or task:HasCompletedTask(147) or task:HasSubmitedTask(147) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000147()
	if GetPlayer():GetTaskMgr():HasCompletedTask(147) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000147(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(147) == npcId and Task_Accept_00000147 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 147
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ææ¶ç©·å¥‡";
	elseif task:GetTaskSubmitNpc(147) == npcId then
		if Task_Submit_00000147() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 147
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ææ¶ç©·å¥‡";
		elseif task:HasAcceptedTask(147) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 147
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ææ¶ç©·å¥‡";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000147_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™ç©·å¥‡åŸæ˜¯å››å‡¶ä¹‹ä¸€ï¼Œä¸æ— åæ°çˆ¶å­äº‰æ–—ï¼Œç”±æ­¤ç›¸è¯†ç‹¼ç‹ˆä¸ºå¥¸ï¼Œåˆç ´äº†æ˜”å¹´ç™½é˜³çœŸäººç¦åˆ¶ä¸‰å°¸å¢“çš„ç¦æ³•ï¼Œä»æ­¤æ›´æ˜¯åš£å¼ ï¼Œä¸è¿‡è¿™ç©·å¥‡ä¹ƒæ˜¯å››å‡¶ä¹‹ä¸€ï¼Œé¢‡æœ‰äº›æœ¬é¢†ï¼Œå°‘ä¾ ä½ é¡»å¾—å°å¿ƒåº”ä»˜æ‰æ˜¯ã€‚";
	action.m_ActionMsg = "ç®¡ä»–ä»€ä¹ˆå¦–é­”é¬¼æ€ªï¼Œæˆ‘åªå‡­æ‰‹ä¸­é£å‰‘ï¼Œè‡ªå«ä»–æœ‰æ¥æ— å›ã€‚";
	return action;
end

function Task_00000147_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ï¼Œå°‘ä¾ çœŸæ˜¯å‰‘æœ¯é«˜å¼ºï¼Œä¸€èº«æ­£æ°”å•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000147_step_table = {
		[1] = Task_00000147_step_01,
		[10] = Task_00000147_step_10,
		};

function Task_00000147_step(step)
	if Task_00000147_step_table[step] ~= nil then
		return Task_00000147_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000147_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000147() then
		return false;
	end
	if not task:AcceptTask(147) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000147_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(147) then
		return false;
	end


	player:AddExp(90000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000147_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(147);
end
