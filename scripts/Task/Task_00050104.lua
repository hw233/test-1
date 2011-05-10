--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00050104()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 8 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50104) or task:HasCompletedTask(50104) or task:HasSubmitedTask(50104) then
		return false;
	end
	if not task:HasSubmitedTask(50103) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00050104()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 8 then
		return false;
	end
	if task:HasAcceptedTask(50104) or task:HasCompletedTask(50104) or task:HasSubmitedTask(50104) then
		return false;
	end
	if not task:HasSubmitedTask(50103) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00050104()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50104) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00050104(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50104) == npcId and Task_Accept_00050104 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50104
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¿µæ„";
	elseif task:GetTaskSubmitNpc(50104) == npcId then
		if Task_Submit_00050104() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50104
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¿µæ„";
		elseif task:HasAcceptedTask(50104) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50104
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¿µæ„";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00050104_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "æ˜¯é¦™éŸ³ç‹åè¯·ä½ æ¥æ‰¾æˆ‘çš„å—ï¼Ÿ";
	action.m_ActionMsg = "æ˜¯çš„ã€‚";
	return action;
end

function Task_00050104_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å…¶å®é¦™éŸ³ç‹åè‡ªå·±ä¹Ÿå¾ˆæ“…é•¿æ­Œèµ‹ï¼Œä¹Ÿæ˜¯æœ‰åçš„ä¹ç¥ï¼Œå´è¿™ä¹ˆçœ‹å¾—èµ·æˆ‘ï¼ŒçœŸæ˜¯è®©æˆ‘å—å® è‹¥æƒŠï¼Œæ—¢ç„¶æ˜¯ä¸ºç‹åæ­»å»çš„å„¿å­æ–‡éš£çš„æ›²å­ï¼Œæˆ‘ä¸”åœ¨å›éŸ³è´é‡Œå½•ä¸Šä¸€é¦–ã€Šå¿µæ„ã€‹ï¼Œå¸Œæœ›ç‹åå’Œæ–‡éš£èƒ½å€Ÿè¿™é¦–æ›²å­å¿ƒçµäº’é€šå§ã€‚";
	action.m_ActionMsg = "æœç„¶æ˜¯äº†ä¸èµ·çš„è°‹å£«å•Šã€‚";
	return action;
end

function Task_00050104_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è´«åƒ§æœ‰ç¤¼ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050104_step_table = {
		[1] = Task_00050104_step_01,
		[2] = Task_00050104_step_02,
		[10] = Task_00050104_step_10,
		};

function Task_00050104_step(step)
	if Task_00050104_step_table[step] ~= nil then
		return Task_00050104_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00050104_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050104() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15048, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(50104) then
		return false;
	end
	task:AddTaskStep(50104);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00050104_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15048,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(50104) then
		return false;
	end

	package:DelItemAll(15048,1);

	player:AddExp(900);
	player:getCoin(650);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00050104_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15048,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50104);
end
