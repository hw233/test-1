--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00061303()
	local player = GetPlayer();
	if player:GetLev() < 53 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61303) or task:HasCompletedTask(61303) or task:HasSubmitedTask(61303) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61302) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61302) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00061303()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 53 then
		return false;
	end
	if task:HasAcceptedTask(61303) or task:HasCompletedTask(61303) or task:HasSubmitedTask(61303) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61302) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61302) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00061303()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61303) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00061303(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61303) == npcId and Task_Accept_00061303 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61303
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ²»ç–—å«å…µ";
	elseif task:GetTaskSubmitNpc(61303) == npcId then
		if Task_Submit_00061303() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61303
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ²»ç–—å«å…µ";
		elseif task:HasAcceptedTask(61303) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61303
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ²»ç–—å«å…µ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00061303_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "é‚£ä½åŒ»å®˜é€æ¥çš„è¿™äº›åˆ›ä¼¤è¯å¯¹æˆ‘ä»¬æ¥è¯´çœŸæ˜¯é›ªä¸­é€ç‚­å•Šï¼Œæˆ‘ä»¬çš„ä¸€äº›å«å…µå› ä¸ºç¼ºå°‘æ²»ä¼¤çš„è¯å·²ç»ç—…äº†å¥½ä¹…äº†ï¼Œéº»çƒ¦å‹‡å£«ä½ é©¬ä¸Šé€ä¸€ç“¶ç»™é»æœå›½åºŸå¢Ÿçš„é•‡é‚ªæŠ¤å«ï¼Œè®©é‚£é‡Œå—ä¼¤çš„å«å…µå¾—åˆ°æ²»ç–—ã€‚";
	action.m_ActionMsg = "æ²¡é—®é¢˜å¸®ä¸»ï¼Œæˆ‘é©¬ä¸Šå»ã€‚";
	return action;
end

function Task_00061303_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¤ªæ„Ÿè°¢ä½ äº†å‹‡å£«ï¼Œæˆ‘ä»¬å—ä¼¤çš„å«å…µç»ˆäºæœ‰æ•‘äº†.";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061303_step_table = {
		[1] = Task_00061303_step_01,
		[10] = Task_00061303_step_10,
		};

function Task_00061303_step(step)
	if Task_00061303_step_table[step] ~= nil then
		return Task_00061303_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00061303_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061303() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15012, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(61303) then
		return false;
	end
	task:AddTaskStep(61303);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00061303_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15012,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(61303) then
		return false;
	end

	package:DelItemAll(15012,1);

	player:AddExp(68000);
	player:getCoin(57000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00061303_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15012,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61303);
end
