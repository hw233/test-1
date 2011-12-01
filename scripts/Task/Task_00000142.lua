--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000142()
	local player = GetPlayer();
	if player:GetLev() < 65 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(142) or task:HasCompletedTask(142) or task:HasSubmitedTask(142) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(141) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(141) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(141) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000142()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 65 then
		return false;
	end
	if task:HasAcceptedTask(142) or task:HasCompletedTask(142) or task:HasSubmitedTask(142) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(141) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(141) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(141) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000142()
	if GetPlayer():GetTaskMgr():HasCompletedTask(142) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000142(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(142) == npcId and Task_Accept_00000142 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 142
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "é’ç‹¼å¦–é“";
	elseif task:GetTaskSubmitNpc(142) == npcId then
		if Task_Submit_00000142() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 142
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "é’ç‹¼å¦–é“";
		elseif task:HasAcceptedTask(142) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 142
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "é’ç‹¼å¦–é“";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000142_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é‚£ä¸‰å°¸å¢“çš„å¦–å­½å›ºç„¶å¯æ¶ï¼Œå¯æ˜¯é‚£äº›å±…å¿ƒè«æµ‹çš„é‚ªé“å¦–äººæ›´æ˜¯ä¸å¯é¥¶æ•ï¼Œæœ‰ä¸ªå¦–é“å«åšå¶åŸ¹ä¸ä»–çš„å¸®å‡¶å¼ ç§€ä¸€èµ·ï¼Œæƒ³è¶æˆ‘ä»¬å¯¹ä»˜å¦–å°¸çš„æ—¶å€™è°‹å–è½©è¾•è‡³å®ï¼Œå¿…é¡»è¦å…ˆæ¶ˆç­ä»–ä»¬ã€‚";
	action.m_ActionMsg = "å¸ˆå¤ªè«æ„ï¼Œæˆ‘è¿™å°±å»æ¶ˆç­ä»–ä»¬ã€‚";
	return action;
end

function Task_00000142_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ï¼Œå°‘ä¾ çœŸæ˜¯å‰‘æœ¯é«˜å¼ºï¼Œä¸€èº«æ­£æ°”å•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000142_step_table = {
		[1] = Task_00000142_step_01,
		[10] = Task_00000142_step_10,
		};

function Task_00000142_step(step)
	if Task_00000142_step_table[step] ~= nil then
		return Task_00000142_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000142_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000142() then
		return false;
	end
	if not task:AcceptTask(142) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000142_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(142) then
		return false;
	end


	player:AddExp(70000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000142_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(142);
end
