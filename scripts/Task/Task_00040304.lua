--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00040304()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40304) or task:HasCompletedTask(40304) or task:HasSubmitedTask(40304) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00040304()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(40304) or task:HasCompletedTask(40304) or task:HasSubmitedTask(40304) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00040304()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40304) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00040304(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40304) == npcId and Task_Accept_00040304 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40304
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ¯”æ­¦è¯•ç‚¼";
	elseif task:GetTaskSubmitNpc(40304) == npcId then
		if Task_Submit_00040304() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40304
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ¯”æ­¦è¯•ç‚¼";
		elseif task:HasAcceptedTask(40304) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40304
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ¯”æ­¦è¯•ç‚¼";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00040304_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ï¼Œä½ è¯¥è¿›å…¥å®æˆ˜è®­ç»ƒäº†ï¼Œå»ç«æŠ€åœºå§ï¼Œé‚£é‡Œæœ‰æˆ‘ä»¬å¤©æ—æ— æ•°çš„å‹‡å£«åœ¨å’Œé­”æ—å¯¹æŠ—ï¼Œ ä½ å»è§è¯†è§è¯†å§ã€‚";
	action.m_ActionMsg = "æˆ‘ä¸ä¼šè®©å¤©æ—å¤±æœ›çš„ã€‚";
	return action;
end

function Task_00040304_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘ç­‰ä½ çš„å¥½æ¶ˆæ¯ï¼Œç”¨ä½ çš„å®åŠ›å»è¯æ˜ä½ è‡ªå·±å§ã€‚";
	action.m_ActionMsg = "è¯·æ”¾å¿ƒã€‚";
	return action;
end

function Task_00040304_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç°åœ¨ï¼Œä½ çœ‹åˆ°æˆ‘ä»¬å¤©æ—çš„åŠ›é‡æœ‰å¤šä¹ˆå¼ºå¤§äº†å§ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040304_step_table = {
		[1] = Task_00040304_step_01,
		[2] = Task_00040304_step_02,
		[10] = Task_00040304_step_10,
		};

function Task_00040304_step(step)
	if Task_00040304_step_table[step] ~= nil then
		return Task_00040304_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00040304_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040304() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40304) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00040304_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40304) then
		return false;
	end


	player:AddExp(2000);
	player:getCoin(10000);
	player:getTael(120);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00040304_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40304);
end
