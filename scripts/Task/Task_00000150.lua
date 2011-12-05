--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000150()
	local player = GetPlayer();
	if player:GetLev() < 70 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(150) or task:HasCompletedTask(150) or task:HasSubmitedTask(150) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(149) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(149) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(149) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000150()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 70 then
		return false;
	end
	if task:HasAcceptedTask(150) or task:HasCompletedTask(150) or task:HasSubmitedTask(150) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(149) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(149) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(149) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000150()
	if GetPlayer():GetTaskMgr():HasCompletedTask(150) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000150(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(150) == npcId and Task_Accept_00000150 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 150
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "é›è¡æ–©è›Ÿ";
	elseif task:GetTaskSubmitNpc(150) == npcId then
		if Task_Submit_00000150() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 150
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "é›è¡æ–©è›Ÿ";
		elseif task:HasAcceptedTask(150) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 150
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "é›è¡æ–©è›Ÿ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000150_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™åªå¦–é²§ç‹¡è¯ˆå¼‚å¸¸ï¼Œæˆ‘åœ¨æ­¤å®ˆå€™äº†æ•°æ—¥ï¼Œå®ƒåªæ˜¯èº²åœ¨æ±Ÿåº•æ·±å¤„ä¸æ•¢è·¯é¢ã€‚çœ¼çœ‹å®ƒåŠ«æœŸå°†è‡³ï¼Œè¿™å­½ç•œä¼°è®¡æŒ‰æºä¸ä½ï¼Œå°±è¦å‡ºæ¥å…´é£ä½œæµªâ€¦â€¦å®ƒæ‰‹ä¸‹ä¸å°‘æ°´æ—å¦–å­½ï¼Œéœ€è¦ä½ åŠ©æˆ‘å°†å®ƒä»¬å°½æ•°é“²é™¤ã€‚";
	action.m_ActionMsg = "æ²¡é—®é¢˜â€¦â€¦å…¶å®æˆ‘æœ€å–œæ¬¢å¸®åŠ©å¦¹å­äº†ã€‚";
	return action;
end

function Task_00000150_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ï¼Œæ­¤é—´äº‹äº†ï¼Œå¯¹ä½ å¯¹æˆ‘éƒ½æ˜¯è«å¤§çš„åŠŸå¾·å•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000150_step_table = {
		[1] = Task_00000150_step_01,
		[10] = Task_00000150_step_10,
		};

function Task_00000150_step(step)
	if Task_00000150_step_table[step] ~= nil then
		return Task_00000150_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000150_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000150() then
		return false;
	end
	if not task:AcceptTask(150) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000150_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(150) then
		return false;
	end


	player:AddExp(90000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000150_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(150);
end
