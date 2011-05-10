--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00070300()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(70300) or task:HasCompletedTask(70300) or task:HasSubmitedTask(70300) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00070300()
	return false;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00070300()
	if GetPlayer():GetTaskMgr():HasCompletedTask(70300) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00070300(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(70300) == npcId and Task_Accept_00070300 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 70300
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "è‡ªåŠ¨å®Œæˆ";
	elseif task:GetTaskSubmitNpc(70300) == npcId then
		if Task_Submit_00070300() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70300
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "è‡ªåŠ¨å®Œæˆ";
		elseif task:HasAcceptedTask(70300) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70300
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "è‡ªåŠ¨å®Œæˆ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00070300_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ ç®€ç›´å¤ªå¸…æ°”äº†ï¼Œæ²¡å»åšéƒ½èƒ½é¢†åˆ°å¥–åŠ±ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00070300_step_table = {
		[10] = Task_00070300_step_10,
		};

function Task_00070300_step(step)
	if Task_00070300_step_table[step] ~= nil then
		return Task_00070300_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00070300_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00070300() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(70300) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00070300_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(70300) then
		local completedData = GetDayTaskCompletedCount(player, 1003);
		local colorData = GetDayTaskFlushColor(player, 1003);
		if completedData.dayTask == 1 and colorData.dayTask == 1 and completedData.count < 10 then
			if DayTaskAward_Item_00001003(10, colorData.color) then
				local ExpVal = 0;
				for count = completedData.count+1, 10 do
					ExpVal = ExpVal + DayTaskAward_Exp_00001003(count, colorData.color);
				end
				local weekday = getWeekDay();
				if weekday == 2 then
					ExpVal = ExpVal * 2;
				end
--				local stage = getActivityStage();
--				if stage == 7 then
--					ExpVal = ExpVal * 2;
--					SendMsg(player, 0x35, "»î¶¯ÆÚ¼ä£¬¾­ÑéÒÑ¼Ó±¶");
--				end
				player:AddExp(ExpVal);
				task:DayTaskSubmit(70300);
			end
		end
		return true;
	end
	return false;
end

--·ÅÆúÈÎÎñ
function Task_00070300_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AutoTaskAbandon(70300);
end
