--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090120()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90120) or task:HasCompletedTask(90120) or task:HasSubmitedTask(90120) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90119) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090120()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90120) or task:HasCompletedTask(90120) or task:HasSubmitedTask(90120) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90119) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090120()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90120) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090120(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90120) == npcId and Task_Accept_00090120 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90120
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "é²œç¾çš„é£Ÿæ";
	elseif task:GetTaskSubmitNpc(90120) == npcId then
		if Task_Submit_00090120() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90120
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "é²œç¾çš„é£Ÿæ";
		elseif task:HasAcceptedTask(90120) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90120
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "é²œç¾çš„é£Ÿæ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090120_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "æˆ‘çš„æ‰‹è‰ºè¶…çº§æ£’ã€‚";
	action.m_ActionMsg = "é‚£ä¹Ÿè¦ç­‰å°è¿‡ä»¥åæ‰çŸ¥é“ï¼Œä½ å¾—èµ¶ç´§åšäº†ï¼Œå¤§ä¼™éƒ½ç­‰ç€å‘¢ã€‚";
	return action;
end

function Task_00090120_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¤œæ‘©åŸçš„å°†å£«ä»¬æƒ³è¦è¡¥å……ä½“åŠ›ï¼Œé‚£å¯å°‘ä¸äº†è½é­„æ¸”äººçš„é±¼è‚‰ã€‚ä½ å»å¸®æˆ‘å¼„20å—æ¥ã€‚";
	action.m_ActionMsg = "å•Šï¼Ÿé‚£äº›åŠäººåŠé±¼çš„å®¶ä¼™ï¼Ÿåƒæ€ªç‰©çš„è‚‰ï¼Ÿæˆ‘æ€ä¹ˆæ„Ÿè§‰æœ‰ç‚¹åèƒƒã€‚";
	return action;
end

function Task_00090120_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™å¯æ˜¯ä¸Šç­‰çš„é±¼è‚‰å•Šï¼Œæˆ‘åšå¥½åä¼šé©¬ä¸Šé€è¿‡å»çš„ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090120_step_table = {
		[1] = Task_00090120_step_01,
		[2] = Task_00090120_step_02,
		[10] = Task_00090120_step_10,
		};

function Task_00090120_step(step)
	if Task_00090120_step_table[step] ~= nil then
		return Task_00090120_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090120_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090120() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90120) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090120_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15106,1) < 20 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(90120) then
		return false;
	end

	package:DelItemAll(15106,1);

	player:AddExp(650000);
	player:getCoin(312000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090120_abandon()
	local package = GetPlayer():GetPackage();
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90120);
end
