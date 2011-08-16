--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000555()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(555) or task:HasCompletedTask(555) or task:HasSubmitedTask(555) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000555()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(555) or task:HasCompletedTask(555) or task:HasSubmitedTask(555) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000555()
	if GetPlayer():GetTaskMgr():HasCompletedTask(555) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000555(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(555) == npcId and Task_Accept_00000555 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 555
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "é‡‡èŠ±å¤§ç›—";
	elseif task:GetTaskSubmitNpc(555) == npcId then
		if Task_Submit_00000555() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 555
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "é‡‡èŠ±å¤§ç›—";
		elseif task:HasAcceptedTask(555) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 555
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "é‡‡èŠ±å¤§ç›—";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000555_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æœ€è¿‘é¢‡æœ‰äº›å¦–äººåœ¨æˆéƒ½ä½œä¹±ï¼Œæˆéƒ½æ²»å®‰çœŸå¾—å¾ˆæˆé—®é¢˜ï¼Œå¬è¯´æœ‰å‡ ä¸ªå¤§æˆ·äººå®¶çš„å°å§éƒ½è¢«é‚£é‡‡èŠ±å¤§ç›—ç³Ÿè¹‹ï¼Œæˆéƒ½çš„å®˜åºœå¯¹æ­¤æŸæ‰‹æ— ç­–ï¼Œå¬è¯´æ˜¯å‰‘ä¾ ä¸€æµåœ¨ä½œæ¡ˆï¼Œ"..GetPlayerName(GetPlayer()).."ä½ å»å°†è¿™ä¸ªæ·«è´¼æ‰æ‹¿å½’æ¡ˆã€‚";
	action.m_ActionMsg = "æˆ‘ç»ä¸ä¼šæ”¾è¿‡è¿™äº›é‡‡èŠ±è´¼ï¼";
	return action;
end

function Task_00000555_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ çœŸæ˜¯èº«æ‰‹ä¸å‡¡å•Šã€‚ ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000555_step_table = {
		[1] = Task_00000555_step_01,
		[10] = Task_00000555_step_10,
		};

function Task_00000555_step(step)
	if Task_00000555_step_table[step] ~= nil then
		return Task_00000555_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000555_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000555() then
		return false;
	end
	if not task:AcceptTask(555) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000555_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(555) then
		return false;
	end


	player:AddExp(8000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000555_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(555);
end
