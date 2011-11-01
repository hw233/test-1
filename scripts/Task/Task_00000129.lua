--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000129()
	local player = GetPlayer();
	if player:GetLev() < 38 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(129) or task:HasCompletedTask(129) or task:HasSubmitedTask(129) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(128) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(128) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(128) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000129()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 38 then
		return false;
	end
	if task:HasAcceptedTask(129) or task:HasCompletedTask(129) or task:HasSubmitedTask(129) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(128) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(128) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(128) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000129()
	if GetPlayer():GetTaskMgr():HasCompletedTask(129) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000129(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(129) == npcId and Task_Accept_00000129 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 129
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¤šæ¬¡åŠ é€Ÿ";
	elseif task:GetTaskSubmitNpc(129) == npcId then
		if Task_Submit_00000129() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 129
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¤šæ¬¡åŠ é€Ÿ";
		elseif task:HasAcceptedTask(129) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 129
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¤šæ¬¡åŠ é€Ÿ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000129_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ ç°åœ¨å°±å¯ä»¥å»å°è¯•ä¸€ä¸‹ï¼Œä»¥ä½ ç°åœ¨çš„æ°´å¹³å»ç¦ä»™æ½­åº•æ¯’çŸ³çš„åœ°æ–¹æŒ‚æœºæ¯”è¾ƒä¸é”™ï¼Œæ²¡æœ‰å±é™©åˆæ¯”è¾ƒçœåŠ›ï¼Œæ­£å¸¸æ¶ˆç­200ä¸ªæ€ªå¯æ˜¯å¾ˆè‰°å·¨çš„ä»»åŠ¡ï¼Œä¸è¿‡ä½ è¯•ç€å»æŒ‚æœºçš„è¯ï¼Œå°±å¾ˆè½»æ¾çœåŠ›äº†ã€‚";
	action.m_ActionMsg = "å¥½çš„ï¼Œæˆ‘è¿™å°±å»ã€‚";
	return action;
end

function Task_00000129_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ çœŸæ˜¯å¥½èº«æ‰‹ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000129_step_table = {
		[1] = Task_00000129_step_01,
		[10] = Task_00000129_step_10,
		};

function Task_00000129_step(step)
	if Task_00000129_step_table[step] ~= nil then
		return Task_00000129_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000129_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000129() then
		return false;
	end
	if not task:AcceptTask(129) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000129_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(129) then
		return false;
	end


	player:AddExp(20000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000129_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(129);
end
