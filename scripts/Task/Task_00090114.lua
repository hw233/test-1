--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090114()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90114) or task:HasCompletedTask(90114) or task:HasSubmitedTask(90114) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090114()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90114) or task:HasCompletedTask(90114) or task:HasSubmitedTask(90114) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090114()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90114) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090114(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90114) == npcId and Task_Accept_00090114 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90114
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç§˜åˆ¶ç¾é…’";
	elseif task:GetTaskSubmitNpc(90114) == npcId then
		if Task_Submit_00090114() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90114
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç§˜åˆ¶ç¾é…’";
		elseif task:HasAcceptedTask(90114) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90114
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç§˜åˆ¶ç¾é…’";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090114_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‹‡å£«ï¼Œæˆ‘å¬è¯´ä½ æ˜¯å¤œæ‘©åŸæ¥çš„ã€‚é‚£æƒ³å¿…ä½ å¯¹é‚£å¾ˆç†Ÿæ‚‰äº†ï¼Œé»‘é£å¯¨ä½ åº”è¯¥çŸ¥é“ï¼Œé‚£é‡Œä½ç€ä¸€ç¾¤å¯æ¶çš„å¼ºç›—ã€‚ä»–ä»¬è·¯è¿‡æˆ‘è¿™çš„æ—¶å€™å±…ç„¶æŠ¢èµ°äº†æˆ‘äº²æ‰‹é…¿åˆ¶çš„ç¾é…’ï¼Œä½ èƒ½å¸®æˆ‘æŠ¢å›æ¥å—ï¼Ÿ";
	action.m_ActionMsg = "ç¾é…’ï¼Ÿè¿™ä¼šè‚¯å®šè¢«é‚£äº›æ€ªç‰©å–æ²¡äº†ï¼Œæˆ‘å°½é‡ç»™ä½ æŠ¢å›ä¸€äº›ã€‚";
	return action;
end

function Task_00090114_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "çœŸæ˜¯å¯æƒœï¼Œåªå‰©è¿™ä¸€å›äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090114_step_table = {
		[1] = Task_00090114_step_01,
		[10] = Task_00090114_step_10,
		};

function Task_00090114_step(step)
	if Task_00090114_step_table[step] ~= nil then
		return Task_00090114_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090114_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090114() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90114) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090114_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15105,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(90114) then
		return false;
	end

	package:DelItemAll(15105,1);

	player:AddExp(625000);
	player:getCoin(310000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090114_abandon()
	local package = GetPlayer():GetPackage();
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90114);
end
