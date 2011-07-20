--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000506()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(506) or task:HasCompletedTask(506) or task:HasSubmitedTask(506) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000506()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(506) or task:HasCompletedTask(506) or task:HasSubmitedTask(506) then
		return false;
	end
	if player:ColorTaskOutOf(0) then
		return false
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000506()
	if GetPlayer():GetTaskMgr():HasCompletedTask(506) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000506(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(506) == npcId and Task_Accept_00000506 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 506
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ°´è´¼å¤´ç›®";
	elseif task:GetTaskSubmitNpc(506) == npcId then
		if Task_Submit_00000506() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 506
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ°´è´¼å¤´ç›®";
		elseif task:HasAcceptedTask(506) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 506
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ°´è´¼å¤´ç›®";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000506_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆéƒ½å‰æ®µæ—¶é—´æ²»å®‰çœŸå¾—å¾ˆæˆé—®é¢˜ï¼Œæœ‰ä¸€è‚¡æ°´è´¼åœ¨é™„è¿‘ä¸ºæ¶ï¼Œä»–ä»¬çš„å¤´ç›®æ˜¯ä¸ªå«æåˆšçš„å®¶ä¼™ï¼Œæ®‹æš´å¼‚å¸¸ï¼Œ"..GetPlayerName(GetPlayer()).."é€Ÿé€Ÿå»å°†è¿™ä¸ªæ¶äººé“²é™¤ã€‚";
	action.m_ActionMsg = "ä¸ºæ°‘é™¤å®³æ˜¯æˆ‘è¾ˆæœ¬è‰²ï¼Œå¼Ÿå­å»å»å°±å›ã€‚";
	return action;
end

function Task_00000506_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ çœŸæ˜¯èº«æ‰‹ä¸å‡¡å•Šã€‚ ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000506_step_table = {
		[1] = Task_00000506_step_01,
		[10] = Task_00000506_step_10,
		};

function Task_00000506_step(step)
	if Task_00000506_step_table[step] ~= nil then
		return Task_00000506_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000506_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000506() then
		return false;
	end
	if not task:AcceptTask(506) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000506_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(506) then
		return false;
	end


	player:AddExp(2222);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000506_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(506);
end
