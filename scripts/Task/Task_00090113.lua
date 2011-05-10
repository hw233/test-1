--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00090113()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90113) or task:HasCompletedTask(90113) or task:HasSubmitedTask(90113) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00090113()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90113) or task:HasCompletedTask(90113) or task:HasSubmitedTask(90113) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00090113()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90113) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00090113(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90113) == npcId and Task_Accept_00090113 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90113
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¤œæ‘©å›½å¸ˆ";
	elseif task:GetTaskSubmitNpc(90113) == npcId then
		if Task_Submit_00090113() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90113
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¤œæ‘©å›½å¸ˆ";
		elseif task:HasAcceptedTask(90113) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90113
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¤œæ‘©å›½å¸ˆ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00090113_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "æ—¢ç„¶å•å¸ƒä¸æ„¿æ„æ¥é‚£ä½ å°±å¸®æˆ‘ä¸ªå¿™å§ã€‚";
	action.m_ActionMsg = "å°†å†›è¯·è®²ã€‚";
	return action;
end

function Task_00090113_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "åœ¨ç¦»è¿™ä¸è¿œçš„ç„å¹»å¡é©»æ‰ç€å¤œæ‘©å›½å¸ˆæ‘©æŸ¯å¿ƒï¼Œä»–æ˜¯æ‘©æŸ¯ä¿±çš„å…„å¼Ÿã€‚ä½ å»æ€äº†æ‘©æŸ¯å¿ƒï¼Œè‡³äºæ‘©æŸ¯ä¿±ï¼Œæœ‰å•å¸ƒåœ¨é‚£çœ‹ç€ï¼Œä¸è¶³ä¸ºæ‚£ã€‚ ";
	action.m_ActionMsg = "å°†å†›ä¸”ç­‰æˆ‘çš„å¥½æ¶ˆæ¯ã€‚";
	return action;
end

function Task_00090113_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½†æ„¿è¿™æ¬¡ä½ æ˜¯çœŸå°†ä»–æ€äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090113_step_table = {
		[1] = Task_00090113_step_01,
		[2] = Task_00090113_step_02,
		[10] = Task_00090113_step_10,
		};

function Task_00090113_step(step)
	if Task_00090113_step_table[step] ~= nil then
		return Task_00090113_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00090113_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090113() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90113) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00090113_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90113) then
		return false;
	end


	player:AddExp(625000);
	player:getCoin(310000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00090113_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90113);
end
