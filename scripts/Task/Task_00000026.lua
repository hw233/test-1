--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000026()
	local player = GetPlayer();
	if player:GetLev() < 11 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(26) or task:HasCompletedTask(26) or task:HasSubmitedTask(26) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(25) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(25) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(25) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000026()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 11 then
		return false;
	end
	if task:HasAcceptedTask(26) or task:HasCompletedTask(26) or task:HasSubmitedTask(26) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(25) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(25) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(25) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000026()
	if GetPlayer():GetTaskMgr():HasCompletedTask(26) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000026(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(26) == npcId and Task_Accept_00000026 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 26
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¼‚äººæ”¶å¾’";
	elseif task:GetTaskSubmitNpc(26) == npcId then
		if Task_Submit_00000026() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 26
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¼‚äººæ”¶å¾’";
		elseif task:HasAcceptedTask(26) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 26
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¼‚äººæ”¶å¾’";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000026_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ¥ä¸‹æ¥å°±è®©ä½ å¸®æˆ‘è·‘è·‘è…¿äº†ï¼Œå’Œä½ ä¸€èµ·çš„ä¸æ˜¯æœ‰ä¸ªå«å‘¨æ·³çš„å—ï¼Œè™½ç„¶å¹´è¿‡å››ååˆä¸æ˜¯ç«¥èº«ï¼Œä½†åˆ«äººè¯´ä¸å¥½çš„è‡ªå·±ååå°±è§‰å¾—å¥½ï¼Œæˆ‘å°±æ˜¯è¦æ”¶å‘¨æ·³åšå¼Ÿå­ï¼Œè¿™æ˜¯å¦å¤–ä¸€æšè¡¥é«“ç›Šå…ƒä¸¹ä½ å»ç¢§å‡åºµé€ç»™ä»–ã€‚";
	action.m_ActionMsg = "å‰è¾ˆçœŸä¹ƒæ˜¯é£å°˜å¼‚äººå•Šã€‚";
	return action;
end

function Task_00000026_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "çœŸæ˜¯æ„Ÿè°¢"..GetPlayerName(GetPlayer()).."ä½ æ¥å‘Šè¯‰æˆ‘è¿™ä¸ªå–œè®¯ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000026_step_table = {
		[1] = Task_00000026_step_01,
		[10] = Task_00000026_step_10,
		};

function Task_00000026_step(step)
	if Task_00000026_step_table[step] ~= nil then
		return Task_00000026_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000026_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000026() then
		return false;
	end
	local package = player:GetPackage();
	local reqGrids = 0;
	reqGrids = reqGrids + package:GetItemUsedGrids(800, 1, 1);
	if reqGrids > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2012, 0);
		return false;
	end
	if not task:AcceptTask(26) then
		return false;
	end
	package:AddItem(800, 1, 1);
	task:AddTaskStep(26);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000026_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(26) then
		return false;
	end


	player:AddExp(6000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000026_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItem(800, 1, 1);
	return GetPlayer():GetTaskMgr():AbandonTask(26);
end
