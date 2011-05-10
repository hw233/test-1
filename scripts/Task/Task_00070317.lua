--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00070317()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(70317) or task:HasCompletedTask(70317) or task:HasSubmitedTask(70317) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(70504) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(70504) then
			return false;
		end
	end
	if task:GetDayTaskCompletedCnt(1004) >= 4 then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00070317()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(70317) or task:HasCompletedTask(70317) or task:HasSubmitedTask(70317) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1004) >= 4 then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(70504) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(70504) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00070317()
	if GetPlayer():GetTaskMgr():HasCompletedTask(70317) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00070317(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(70317) == npcId and Task_Accept_00070317 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 70317
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "äººäº‰ä¸€å£æ°”";
	elseif task:GetTaskSubmitNpc(70317) == npcId then
		if Task_Submit_00070317() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70317
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "äººäº‰ä¸€å£æ°”";
		elseif task:HasAcceptedTask(70317) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70317
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "äººäº‰ä¸€å£æ°”";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00070317_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¤©æ± ç å¤´åºŸå¢Ÿçš„æ¸”äººå¤´ç›®ï¼Œä»—ç€è‡ªå·±èº«å¼ºåŠ›å£®ï¼Œä¸”è€³ç›®ä¼—å¤šï¼Œå‡ ä¹åœ¨å¤©æ± ç§°éœ¸ä¸€æ–¹äº†ï¼Œæˆ‘ä»¬æ€ä¹ˆå¯ä»¥å®¹å¿ä¸€ä¸ªæ¸”äººè¿™æ ·åšï¼Œå»æŠŠä»–æ€æ‰å§ï¼Œæ— è®ºæ€æ ·äººéƒ½è¦äº‰ä¸€å£æ°”ã€‚";
	action.m_ActionMsg = "åˆæ˜¯é‚£äº›åŠäººåŠé±¼çš„å®¶ä¼™ï¼Œè¿˜çœŸæ˜¯ä¸æ­»å¿ƒï¼Œæˆ‘è¿™å°±å»ã€‚";
	return action;
end

function Task_00070317_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "äººäº‰ä¸€å£æ°”ï¼Œä½›å—ä¸€æŸ±é¦™ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00070317_step_table = {
		[1] = Task_00070317_step_01,
		[10] = Task_00070317_step_10,
		};

function Task_00070317_step(step)
	if Task_00070317_step_table[step] ~= nil then
		return Task_00070317_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00070317_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00070317() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(70317) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00070317_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	local awardBox = { 9027, 9028, 9029, 9030 };
	local awardBoxId = awardBox[math.random(1, #awardBox)];
	local usedGrid = package:GetItemUsedGrids(awardBoxId, 1, 1);
	if usedGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(70317) then
		return false;
	end

	package:AddItem(awardBoxId, 1, 1);

	local task = player:GetTaskMgr();
	task:DelSubmitedTask(70315);
	task:DelSubmitedTask(70503);
	task:DelSubmitedTask(70504);
	task:DelSubmitedTask(70317);
	task:DayTaskSubmit2(1004);
	if Task_Can_Accept_00070315() then
		task:AddCanAcceptTask2(70315, true);
	end
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00070317_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(70317);
end
