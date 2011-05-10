--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00050702()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 21 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50702) or task:HasCompletedTask(50702) or task:HasSubmitedTask(50702) then
		return false;
	end
	if not task:HasSubmitedTask(50701) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00050702()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 21 then
		return false;
	end
	if task:HasAcceptedTask(50702) or task:HasCompletedTask(50702) or task:HasSubmitedTask(50702) then
		return false;
	end
	if not task:HasSubmitedTask(50701) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00050702()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50702) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00050702(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50702) == npcId and Task_Accept_00050702 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50702
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ¸…é™¤æ€ªç‰©";
	elseif task:GetTaskSubmitNpc(50702) == npcId then
		if Task_Submit_00050702() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50702
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ¸…é™¤æ€ªç‰©";
		elseif task:HasAcceptedTask(50702) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50702
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ¸…é™¤æ€ªç‰©";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00050702_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å“¦å±…ç„¶çœŸçš„æ˜¯é‚£ç¾¤å¯æ¶çš„ç‹‚è›®çŒäººï¼Œè¿˜å¥½æˆ‘æ²¡æœ‰è‡ªå·±å»ï¼Œæˆ‘éå¸¸å®³æ€•é‚£ç§ç–¯ç‹‚çš„çŒäººï¼Œè€Œä¸”æˆ‘çš„èŒ¶å¶è¿˜æ²¡æœ‰å®Œå…¨å¼„å¥½ï¼ŒçœŸæ˜¯ä¸å¥½æ„æ€è¿˜å¾—éº»çƒ¦å‹‡å£«ä½ å†å»ä¸€æ¬¡äº†ã€‚";
	action.m_ActionMsg = "å”‰è¿˜çœŸæ˜¯éº»çƒ¦ï¼Œåªå¥½å¸®ä½ å»æ€æ‰ä»–ä»¬äº†ã€‚";
	return action;
end

function Task_00050702_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "çœŸæ˜¯æ„Ÿè°¢ä½ çš„å¸®å¿™å•Šå‹‡å£«ï¼Œç°åœ¨æˆ‘çš„å±±èŒ¶ç»ˆäºå¤„ç†å¥½äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050702_step_table = {
		[1] = Task_00050702_step_01,
		[10] = Task_00050702_step_10,
		};

function Task_00050702_step(step)
	if Task_00050702_step_table[step] ~= nil then
		return Task_00050702_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00050702_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050702() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50702) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00050702_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(50702) then
		return false;
	end


	player:AddExp(1800);
	player:getCoin(3600);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00050702_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50702);
end
