--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000651()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(651) or task:HasCompletedTask(651) or task:HasSubmitedTask(651) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000651()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(651) or task:HasCompletedTask(651) or task:HasSubmitedTask(651) or not player:isClanTask(651) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000651()
	if GetPlayer():GetTaskMgr():HasCompletedTask(651) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000651(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(651) == npcId and Task_Accept_00000651 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 651
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "èœ€ä¸­é—ç¥¸";
	elseif task:GetTaskSubmitNpc(651) == npcId then
		if Task_Submit_00000651() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 651
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "èœ€ä¸­é—ç¥¸";
		elseif task:HasAcceptedTask(651) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 651
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "èœ€ä¸­é—ç¥¸";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000651_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ˜”å¹´å¼ çŒ®å¿ è¼æ¯’èœ€ä¸­ï¼Œç»™å››å·ç™¾å§“å¸¦æ¥æ— ç©·çš„ç¾ç¥¸ï¼Œè™½ç„¶ç°åœ¨è´¼é¦–å·²æ®ï¼Œä½†æ˜¯ä»æœ‰ä¸å°‘è´¼å…µå››å¤„æµçªœï¼Œå‰ç«¯æ—¶é—´ä¸€å°è‚¡è´¼å…µè·‘åˆ°äº†ç™½å °æ‘é™„è¿‘ï¼Œè¿˜å¸Œæœ›å°‘ä¾ å¯ä»¥å¸®å¿™é™¤æ‰ä»–ä»¬ã€‚";
	action.m_ActionMsg = "æˆ‘è¿™å°±å»ã€‚";
	return action;
end

function Task_00000651_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ çœŸæ˜¯èº«æ‰‹ä¸å‡¡å•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000651_step_table = {
		[1] = Task_00000651_step_01,
		[10] = Task_00000651_step_10,
		};

function Task_00000651_step(step)
	if Task_00000651_step_table[step] ~= nil then
		return Task_00000651_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000651_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000651() then
		return false;
	end
	if not task:AcceptTask(651) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000651_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(651) then
		return false;
	end


	player:AddExp(2000);
	player:AddClanBuilding(200);
	player:AddClanContrib(200);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000651_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(651);
end
