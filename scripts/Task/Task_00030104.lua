--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00030104()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30104) or task:HasCompletedTask(30104) or task:HasSubmitedTask(30104) then
		return false;
	end
	if not task:HasSubmitedTask(30103) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00030104()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(30104) or task:HasCompletedTask(30104) or task:HasSubmitedTask(30104) then
		return false;
	end
	if not task:HasSubmitedTask(30103) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00030104()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30104) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00030104(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30104) == npcId and Task_Accept_00030104 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30104
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ…°é—®è¾¹é˜²";
	elseif task:GetTaskSubmitNpc(30104) == npcId then
		if Task_Submit_00030104() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30104
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ…°é—®è¾¹é˜²";
		elseif task:HasAcceptedTask(30104) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30104
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ…°é—®è¾¹é˜²";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00030104_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘ä»¬å‡»é€€äº†é‚£äº›ç–¯ç‹‚çš„é­”æ—å¤ä»‡è€…ï¼Œä¹Ÿç®—æ˜¯ä¸ªå¥½æ¶ˆæ¯ï¼Œä½ å»è¾¹é˜²é‚£é‡Œå‘ŠçŸ¥å£«å…µä»¬è¿™ä¸ªæ¶ˆæ¯å§ï¼Œé¡ºä¾¿å†å¸¦å»ä¸€äº›ä»–ä»¬éœ€è¦çš„ç‰©èµ„ï¼Œå¸®åŠ©ä»–ä»¬æ›´å¥½çš„é˜²å®ˆæˆ‘ä»¬çš„åŠ¿åŠ›ã€‚";
	action.m_ActionMsg = "å¥½çš„ï¼Œæˆ‘ç°åœ¨å»ã€‚";
	return action;
end

function Task_00030104_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ„Ÿè°¢æ‚¨å¸¦æ¥çš„æ¶ˆæ¯å’Œç‰©èµ„ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030104_step_table = {
		[1] = Task_00030104_step_01,
		[10] = Task_00030104_step_10,
		};

function Task_00030104_step(step)
	if Task_00030104_step_table[step] ~= nil then
		return Task_00030104_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00030104_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030104() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15032, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(30104) then
		return false;
	end
	task:AddTaskStep(30104);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00030104_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15032,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(30104) then
		return false;
	end

	package:DelItemAll(15032,1);

	player:AddExp(4000);
	player:getCoin(5500);
	player:getTael(10);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00030104_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15032,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30104);
end
