--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00070315()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(70315) or task:HasCompletedTask(70315) or task:HasSubmitedTask(70315) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1004) >= 4 then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00070315()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(70315) or task:HasCompletedTask(70315) or task:HasSubmitedTask(70315) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1004) >= 4 then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00070315()
	if GetPlayer():GetTaskMgr():HasCompletedTask(70315) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00070315(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(70315) == npcId and Task_Accept_00070315 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 70315
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "çƒ½çƒŸå†èµ·";
	elseif task:GetTaskSubmitNpc(70315) == npcId then
		if Task_Submit_00070315() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70315
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "çƒ½çƒŸå†èµ·";
		elseif task:HasAcceptedTask(70315) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70315
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "çƒ½çƒŸå†èµ·";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00070315_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‰ä¸¤å¤©å‡ºå»æ•£æ­¥ï¼Œè·¯è¿‡ç‹èˆåŸåºŸå¢Ÿçš„é»‘é£å²—ï¼Œå·®ç‚¹è¢«é‚£é‡Œçš„ä¸€äº›æ€ªå…½å“æ­»ï¼Œå¹¸å¥½æˆ‘è·‘çš„å¿«ï¼Œåæ¥çŸ¥é“é‚£äº›å¯æ€•çš„æ€ªå…½å«é»‘é£å…½ï¼Œå¦‚æœä½ ç°åœ¨æœ‰æ—¶é—´ï¼Œå°±å»å¸®æˆ‘æ€æ‰ä»–ä»¬å§ï¼Œè²Œä¼¼æœ€è¿‘æˆ˜ç«ä¸æ–­ï¼Œçƒ½çƒŸå†èµ·ï¼Œæˆ‘ä»¬ä¸èƒ½å†é—²ç€äº†ã€‚";
	action.m_ActionMsg = "æˆ‘ä¸€ç›´å°±æ²¡é—²ç€ï¼Œæˆ‘èƒ†å­å¯æ¯”ä½ å¤§ã€‚";
	return action;
end

function Task_00070315_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ˜ç«ä¸æ–­ï¼Œçƒ½çƒŸå†èµ·â€¦â€¦";
	action.m_ActionMsg = "";
	return action;
end

local Task_00070315_step_table = {
		[1] = Task_00070315_step_01,
		[10] = Task_00070315_step_10,
		};

function Task_00070315_step(step)
	if Task_00070315_step_table[step] ~= nil then
		return Task_00070315_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00070315_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00070315() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(70315) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00070315_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(70315) then
		return false;
	end


	player:getCoin(200000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00070315_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(70315);
end
