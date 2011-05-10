--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00020405()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 22 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20405) or task:HasCompletedTask(20405) or task:HasSubmitedTask(20405) then
		return false;
	end
	if not task:HasSubmitedTask(20404) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00020405()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 22 then
		return false;
	end
	if task:HasAcceptedTask(20405) or task:HasCompletedTask(20405) or task:HasSubmitedTask(20405) then
		return false;
	end
	if not task:HasSubmitedTask(20404) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00020405()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20405) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00020405(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20405) == npcId and Task_Accept_00020405 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20405
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "è¯¢é—®ä½¿èŠ‚";
	elseif task:GetTaskSubmitNpc(20405) == npcId then
		if Task_Submit_00020405() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20405
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "è¯¢é—®ä½¿èŠ‚";
		elseif task:HasAcceptedTask(20405) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20405
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "è¯¢é—®ä½¿èŠ‚";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00020405_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "ä½ æ˜¯ä¸ºç”˜éœ²çš„äº‹æƒ…æ¥çš„ï¼Ÿè¿™æ¬¡çš„ç”˜éœ²æœ‰é—®é¢˜å—ï¼Ÿ";
	action.m_ActionMsg = "æ˜¯çš„ï¼Œéš¾é™€ç½—å¤§é¾™ç‹è¯´ç”˜éœ²çš„æ•°é‡åªæœ‰å¹³æ—¶çš„ä¸€åŠã€‚";
	return action;
end

function Task_00020405_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ€ä¹ˆä¼šè¿™æ ·ï¼æˆ‘å»å¤©åº­å–ç”˜éœ²çš„æ—¶å€™ä»€ä¹ˆäº‹æƒ…ä¹Ÿæ²¡æœ‰å•Šï¼Œéš¾é“æ˜¯å¸é‡Šå¤©å¼€å§‹åæ‚”äº†ï¼Ÿ";
	action.m_ActionMsg = "çœ‹æ¥ä½ ä¹Ÿä¸çŸ¥é“æ˜¯æ€ä¹ˆå›äº‹äº†ã€‚";
	return action;
end

function Task_00020405_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½¿èŠ‚å‘Šè¯‰ä½ è¿™æ˜¯æ€ä¹ˆå›äº‹äº†å—ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020405_step_table = {
		[1] = Task_00020405_step_01,
		[2] = Task_00020405_step_02,
		[10] = Task_00020405_step_10,
		};

function Task_00020405_step(step)
	if Task_00020405_step_table[step] ~= nil then
		return Task_00020405_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00020405_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020405() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20405) then
		return false;
	end
	task:AddTaskStep(20405);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00020405_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(20405) then
		return false;
	end


	player:AddExp(2200);
	player:getCoin(3700);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00020405_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20405);
end
