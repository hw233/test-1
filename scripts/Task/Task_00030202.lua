--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00030202()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30202) or task:HasCompletedTask(30202) or task:HasSubmitedTask(30202) then
		return false;
	end
	if not task:HasSubmitedTask(30201) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00030202()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(30202) or task:HasCompletedTask(30202) or task:HasSubmitedTask(30202) then
		return false;
	end
	if not task:HasSubmitedTask(30201) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00030202()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30202) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00030202(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30202) == npcId and Task_Accept_00030202 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30202
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¤©æ—å¤ä»‡è€…";
	elseif task:GetTaskSubmitNpc(30202) == npcId then
		if Task_Submit_00030202() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30202
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¤©æ—å¤ä»‡è€…";
		elseif task:HasAcceptedTask(30202) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30202
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¤©æ—å¤ä»‡è€…";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00030202_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç°åœ¨æˆ‘ä»¬é­”æ—çš„æ£‹å­è¢«ç«‹å¥½ä»¥åï¼Œä¸€å®šä¼šæœ‰å¾ˆå¤šç–¯ç‹‚çš„å¤©æ—å¤ä»‡è€…èšé›†åœ¨è§‚æ——å°å¤–é¢çš„ç©ºåœ°ä¸Šçš„ï¼Œæˆ‘å¸Œæœ›ä½ å»å¸®æˆ‘æŸ¥çœ‹ä¸€ä¸‹ï¼Œä¸è¿‡åœ¨äº†è§£ä»–ä»¬çš„å®åŠ›ä¹‹å‰ï¼Œä¸è¦è´¸ç„¶åŠ¨æ‰‹ã€‚";
	action.m_ActionMsg = "å¥½çš„ï¼Œæˆ‘å»çœ‹çœ‹ã€‚";
	return action;
end

function Task_00030202_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é‚£é‡Œçš„æƒ…å†µæ€ä¹ˆæ ·ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030202_step_table = {
		[1] = Task_00030202_step_01,
		[10] = Task_00030202_step_10,
		};

function Task_00030202_step(step)
	if Task_00030202_step_table[step] ~= nil then
		return Task_00030202_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00030202_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030202() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30202) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00030202_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30202) then
		return false;
	end


	player:AddExp(4000);
	player:getCoin(5500);
	player:getTael(10);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00030202_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30202);
end
