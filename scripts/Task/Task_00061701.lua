--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00061701()
	local player = GetPlayer();
	if player:GetLev() < 63 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61701) or task:HasCompletedTask(61701) or task:HasSubmitedTask(61701) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00061701()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 63 then
		return false;
	end
	if task:HasAcceptedTask(61701) or task:HasCompletedTask(61701) or task:HasSubmitedTask(61701) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00061701()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61701) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00061701(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61701) == npcId and Task_Accept_00061701 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61701
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "é†‰é¬¼è¦å€º";
	elseif task:GetTaskSubmitNpc(61701) == npcId then
		if Task_Submit_00061701() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61701
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "é†‰é¬¼è¦å€º";
		elseif task:HasAcceptedTask(61701) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61701
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "é†‰é¬¼è¦å€º";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00061701_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "å‹‡å£«å¿«è¿‡æ¥ä¸€ä¸‹ï¼Œéº»çƒ¦ä½ ä¸ªäº‹æƒ…ï¼Œâ€¦â€¦é¢â€¦â€¦ï¼Œéº»çƒ¦ä½ ä¸ªäº‹æƒ…ã€‚";
	action.m_ActionMsg = "ä»€ä¹ˆäº‹å•Šï¼Œä½ è¿™ä¸€èº«é…’æ°”çš„é†‰é¬¼ï¼Ÿ";
	return action;
end

function Task_00061701_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ²¡â€¦â€¦æ²¡é’±ä¹°é…’äº†ï¼Œæˆ‘çš„é’±è¢«äººå€Ÿèµ°äº†ï¼Œä½ å¸®æˆ‘å»â€¦â€¦å»æ²³è¥¿å†°é“çš„æŒªæ‹‰å¯ºæ‰¾æ¢…çŠ¹å‰æŠŠæˆ‘çš„é’±è¦å›æ¥å§ã€‚";
	action.m_ActionMsg = "çœŸæ˜¯â€¦â€¦åªå¥½å¸®ä½ äº†ã€‚";
	return action;
end

function Task_00061701_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ è¿™â€¦â€¦ä½ æ˜¯è°å•Šï¼Ÿä¸è¦æ‰¾æˆ‘ï¼Œæˆ‘æ²¡é’±ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061701_step_table = {
		[1] = Task_00061701_step_01,
		[2] = Task_00061701_step_02,
		[10] = Task_00061701_step_10,
		};

function Task_00061701_step(step)
	if Task_00061701_step_table[step] ~= nil then
		return Task_00061701_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00061701_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061701() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61701) then
		return false;
	end
	task:AddTaskStep(61701);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00061701_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(61701) then
		return false;
	end


	player:AddExp(160000);
	player:getCoin(130000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00061701_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61701);
end
