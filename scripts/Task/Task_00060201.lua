--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00060201()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 31 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60201) or task:HasCompletedTask(60201) or task:HasSubmitedTask(60201) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00060201()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 31 then
		return false;
	end
	if task:HasAcceptedTask(60201) or task:HasCompletedTask(60201) or task:HasSubmitedTask(60201) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00060201()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60201) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00060201(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60201) == npcId and Task_Accept_00060201 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60201
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç¥ç§˜ä¿¡å°";
	elseif task:GetTaskSubmitNpc(60201) == npcId then
		if Task_Submit_00060201() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60201
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç¥ç§˜ä¿¡å°";
		elseif task:HasAcceptedTask(60201) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60201
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
function Task_00060201_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é­”æ—å‰çº¿å“¨æ‰€çš„ä¼ é€é—¨ï¼Œå¯ä»¥é€šå¾€åœ°ç•Œã€‚ä½ ä»¬ç°åœ¨è¦é›†ç»“éƒ¨é˜Ÿï¼Œç”±å·¨çŸ³å †çš„å·¨å‹ç‹¼äººæ¥æ¥åº”ï¼Œé›†ä½“æ”»é€šè¿‡ä¼ é€é—¨ä»¥åï¼Œä»é¬¼èœ®å±±æ‰“åˆ°ä»€åˆ¹æµ·ï¼Œä»¥ååœ°ç•Œå°±æ˜¯æˆ‘ä»¬çš„äº†ã€‚";
	action.m_ActionMsg = "å±…ç„¶åœ¨å¯†è°‹å é¢†æˆ‘ä»¬çš„åœ°ç•Œï¼";
	return action;
end

function Task_00060201_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‹‡å£«ï¼Œä½ è¯´è¿™ä¸ªæ˜¯ç‹¼é¬¼æ—çš„è¿›æ”»è®¡åˆ’ï¼Ÿ ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060201_step_table = {
		[1] = Task_00060201_step_01,
		[10] = Task_00060201_step_10,
		};

function Task_00060201_step(step)
	if Task_00060201_step_table[step] ~= nil then
		return Task_00060201_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00060201_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060201() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60201) then
		return false;
	end
	task:AddTaskStep(60201);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00060201_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15001,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(60201) then
		return false;
	end

	package:DelItemAll(15001,1);

	player:AddExp(4000);
	player:getCoin(5600);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00060201_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15001,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60201);
end
