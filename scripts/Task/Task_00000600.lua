--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000600()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(600) or task:HasCompletedTask(600) or task:HasSubmitedTask(600) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000600()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(600) or task:HasCompletedTask(600) or task:HasSubmitedTask(600) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000600()
	if GetPlayer():GetTaskMgr():HasCompletedTask(600) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000600(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(600) == npcId and Task_Accept_00000600 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 600
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "åƒµå°¸ä½œä¹±";
	elseif task:GetTaskSubmitNpc(600) == npcId then
		if Task_Submit_00000600() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 600
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "åƒµå°¸ä½œä¹±";
		elseif task:HasAcceptedTask(600) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 600
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "åƒµå°¸ä½œä¹±";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000600_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è‡ªä»ä¸­åŸé­é‡å…µåŠ«ï¼Œæ°‘ä¸èŠç”Ÿï¼Œä¸ä½†æœ‰æ¶äººä½œä¹±ï¼Œç”šè‡³æœ‰äº›åœ¨å…µåŠ«ä¸­æƒ¨é­æ¨ªæ­»çš„äººæ‰§å¿µæ·±é‡ï¼ŒåŒ–ä¸ºåƒµå°¸ä¸ºæ¶ï¼Œè¿‘æ—¥åœ¨æˆéƒ½éƒŠå¤–ä¹±è‘¬å²—å¤„æœ‰æ‘æ°‘å‘ç°äº†ä¸€äº›å¯æ€•çš„åƒµå°¸ï¼Œè¿˜å¸Œæœ›"..GetPlayerName(GetPlayer()).."å»å°†è¿™äº›æ€ªç‰©æ¶ˆç­ã€‚";
	action.m_ActionMsg = "å‡ ä¸ªåƒµå°¸ä¸åœ¨è¯ä¸‹ï¼Œæˆ‘å»å»å°±å›ã€‚";
	return action;
end

function Task_00000600_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ æœç„¶æ˜¯èº«æ‰‹ä¸å‡¡ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000600_step_table = {
		[1] = Task_00000600_step_01,
		[10] = Task_00000600_step_10,
		};

function Task_00000600_step(step)
	if Task_00000600_step_table[step] ~= nil then
		return Task_00000600_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000600_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000600() then
		return false;
	end
	if not task:AcceptTask(600) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000600_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(600) then
		return false;
	end


	player:AddExp(3000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000600_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(600);
end
