--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00062206()
	local player = GetPlayer();
	if player:GetLev() < 38 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(62206) or task:HasCompletedTask(62206) or task:HasSubmitedTask(62206) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00062206()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 38 then
		return false;
	end
	if task:HasAcceptedTask(62206) or task:HasCompletedTask(62206) or task:HasSubmitedTask(62206) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00062206()
	if GetPlayer():GetTaskMgr():HasCompletedTask(62206) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00062206(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(62206) == npcId and Task_Accept_00062206 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 62206
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "åŠ å…¥å®—æ—";
	elseif task:GetTaskSubmitNpc(62206) == npcId then
		if Task_Submit_00062206() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62206
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "åŠ å…¥å®—æ—";
		elseif task:HasAcceptedTask(62206) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62206
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "åŠ å…¥å®—æ—";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00062206_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "å­¤èº«ä¸€äººï¼Œç»ƒè‡³æ­¤å¢ƒç•Œæ˜¯å¦å€æ„Ÿå¯‚å¯ï¼Ÿ";
	action.m_ActionMsg = "ä½ æƒ³å¹²å˜›ï¼Ÿï¼ˆèƒŒåä¸€ä¸å‡‰é£å¹è¿‡ï¼‰";
	return action;
end

function Task_00062206_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å“ˆå“ˆ~~å‹‡å£«åˆ«è¯¯ä¼šï¼Œæˆ‘åªæ˜¯æƒ³è¯´å±±æµ·å¤§é™†ä¸Šçš„å®—æ—æœŸå¾…ä½ çš„åŠ å…¥ï¼Œè¯·é€Ÿå»é€‰æ‹©ä¸€ä¸ªå–œæ¬¢çš„å®—æ—åŠ å…¥ï¼ˆç‚¹å‡»å®—æ—æŒ‰é’®ï¼Œé€‰æ‹©å®—æ—ï¼Œç‚¹å‡»åŠ äººï¼‰";
	action.m_ActionMsg = "åŸæ¥æ˜¯è¿™æ ·ï¼Œé‚£æˆ‘è¿™å°±å»ã€‚";
	return action;
end

function Task_00062206_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä¸å†å¯‚å¯äº†å§ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00062206_step_table = {
		[1] = Task_00062206_step_01,
		[2] = Task_00062206_step_02,
		[10] = Task_00062206_step_10,
		};

function Task_00062206_step(step)
	if Task_00062206_step_table[step] ~= nil then
		return Task_00062206_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00062206_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00062206() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(62206) then
		return false;
	end
	if player:getClan() ~= nil then
		task:AddTaskStep(62206);
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00062206_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(62206) then
		return false;
	end


	player:AddExp(10000);
	player:getCoin(20000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00062206_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(62206);
end
