--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00060101()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 31 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60101) or task:HasCompletedTask(60101) or task:HasSubmitedTask(60101) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00060101()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 31 then
		return false;
	end
	if task:HasAcceptedTask(60101) or task:HasCompletedTask(60101) or task:HasSubmitedTask(60101) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00060101()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60101) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00060101(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60101) == npcId and Task_Accept_00060101 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60101
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç¥ç§˜ä¿¡å°";
	elseif task:GetTaskSubmitNpc(60101) == npcId then
		if Task_Submit_00060101() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60101
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç¥ç§˜ä¿¡å°";
		elseif task:HasAcceptedTask(60101) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60101
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç¥ç§˜ä¿¡å°";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00060101_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¤©æ—å‰çº¿è¥åœ°çš„ä¼ é€é—¨ï¼Œå¯ä»¥é€šå¾€å¤©åº­ï¼Œä½ ä»¬ç°åœ¨è¦é›†ç»“éƒ¨é˜Ÿï¼Œç”±ä¹±çŸ³é˜µçš„å·¨å‹ç‹¼äººæ¥æ¥åº”ï¼Œé›†ä½“é€šè¿‡ä¼ é€é—¨ä»¥åï¼Œä»è§‚éŸ³å±±æ‰“åˆ°é¡»å¼¥å±±ï¼Œä»¥åå¤©åº­å°±æ˜¯æˆ‘ä»¬çš„äº†ã€‚";
	action.m_ActionMsg = "å±…ç„¶åœ¨å¯†è°‹å é¢†æˆ‘ä»¬çš„å¤©åº­ï¼";
	return action;
end

function Task_00060101_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‹‡å£«ï¼Œä½ è¯´è¿™ä¸ªæ˜¯ç‹¼é¬¼æ—çš„è¿›æ”»è®¡åˆ’ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060101_step_table = {
		[1] = Task_00060101_step_01,
		[10] = Task_00060101_step_10,
		};

function Task_00060101_step(step)
	if Task_00060101_step_table[step] ~= nil then
		return Task_00060101_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00060101_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060101() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60101) then
		return false;
	end
	task:AddTaskStep(60101);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00060101_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15001,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(60101) then
		return false;
	end

	package:DelItemAll(15001,1);

	player:AddExp(4000);
	player:getCoin(5600);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00060101_abandon()
	local package = GetPlayer():GetPackage();
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60101);
end
