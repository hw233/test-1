--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00010303()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 18 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10303) or task:HasCompletedTask(10303) or task:HasSubmitedTask(10303) then
		return false;
	end
	if not task:HasSubmitedTask(10302) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00010303()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 18 then
		return false;
	end
	if task:HasAcceptedTask(10303) or task:HasCompletedTask(10303) or task:HasSubmitedTask(10303) then
		return false;
	end
	if not task:HasSubmitedTask(10302) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00010303()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10303) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00010303(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10303) == npcId and Task_Accept_00010303 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10303
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ä¸ºç‹å¦ƒé€ä¿¡";
	elseif task:GetTaskSubmitNpc(10303) == npcId then
		if Task_Submit_00010303() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10303
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ä¸ºç‹å¦ƒé€ä¿¡";
		elseif task:HasAcceptedTask(10303) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10303
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ä¸ºç‹å¦ƒé€ä¿¡";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00010303_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "æ²¡æƒ³åˆ°ä½ å±…ç„¶å¸®æˆ‘å¸¦æ¥è¿™ä¹ˆç§˜å¯†çš„ä¸œè¥¿ï¼Œç›¸ä¿¡ä½ ä¹Ÿè§è¿‡å¹¶ä¸”çŸ¥é“é€è¿™ä¸ªä¸œè¥¿æ¥çš„äººåœ¨å“ªé‡Œäº†ã€‚";
	action.m_ActionMsg = "æ˜¯çš„ï¼Œç‹å¦ƒï¼Œæˆ‘å·²ç»è§è¿‡é‚£ä¸ªäººäº†ã€‚";
	return action;
end

function Task_00010303_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é‚£ä¹ˆå°±è¯·ä½ å†è·‘ä¸€è¶Ÿï¼ŒæŠŠè¿™å°ä¿¡é€ç»™é‚£ä¸ªè¯·ä½ æ¥è§æˆ‘çš„äººã€‚";
	action.m_ActionMsg = "åˆè®©æˆ‘å›å»å•Šâ€¦â€¦è¿™â€¦â€¦å¥½å§ã€‚";
	return action;
end

function Task_00010303_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆ‘çš„å‹‡å£«ï¼Œä½ å±…ç„¶å¸¦æ¥äº†æ‚¦æ„ç‹å¦ƒçš„å¯†ä¿¡ï¼Ÿè®©æˆ‘çœ‹çœ‹";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010303_step_table = {
		[1] = Task_00010303_step_01,
		[2] = Task_00010303_step_02,
		[10] = Task_00010303_step_10,
		};

function Task_00010303_step(step)
	if Task_00010303_step_table[step] ~= nil then
		return Task_00010303_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00010303_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010303() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15022, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(10303) then
		return false;
	end
	task:AddTaskStep(10303);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00010303_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15022,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(10303) then
		return false;
	end

	package:DelItemAll(15022,1);

	player:AddExp(1800);
	player:getCoin(1900);
	player:getTael(10);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00010303_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15022,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10303);
end
