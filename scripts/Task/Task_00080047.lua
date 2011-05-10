--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00080047()
	local player = GetPlayer();
	if player:GetLev() < 47 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80047) or task:HasCompletedTask(80047) or task:HasSubmitedTask(80047) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00080047()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 47 then
		return false;
	end
	if task:HasAcceptedTask(80047) or task:HasCompletedTask(80047) or task:HasSubmitedTask(80047) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00080047()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80047) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00080047(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80047) == npcId and Task_Accept_00080047 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80047
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç”šåš£å°˜ä¸Š";
	elseif task:GetTaskSubmitNpc(80047) == npcId then
		if Task_Submit_00080047() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80047
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç”šåš£å°˜ä¸Š";
		elseif task:HasAcceptedTask(80047) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80047
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç”šåš£å°˜ä¸Š";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00080047_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‹‡å£«ï¼Œä¿®ç‚¼çš„å†ç¨‹æ˜¯è‰°è¾›è€Œæ›²æŠ˜çš„ï¼Œå±±æµ·å¤§é™†çš„æƒ³å¿…ä½ å·²ç»æœ‰æ‰€äº†è§£ã€‚æˆ‘æ€¥éœ€ä½ çš„å¸®åŠ©ï¼Œå»æ€æ‰é‚£äº›åœ¨ç å¤´åºŸå¢Ÿçš„é™†ä¸Šé£Ÿäººé±¼ï¼Œæ”¶é›†15ä¸ªä»–ä»¬çš„é—ªå…‰é±¼é³ï¼Œæˆ‘ç°åœ¨ä¸å¾—ä¸å€ŸåŠ©ä½ çš„åŠ›é‡ï¼Œæ¥å¹³æ¯è¿™æ ·çš„çŠ¶å†µï¼Œè€Œä¸”è¿™åŒæ—¶èƒ½ç»™ä½ çš„ä¿®ç‚¼å¸¦æ¥æ”¶è·ã€‚";
	action.m_ActionMsg = "çš„ç¡®æ˜¯ä¸é”™çš„ä¸»æ„ã€‚";
	return action;
end

function Task_00080047_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "çš„ç¡®æ˜¯ä¸€ä¸¾ä¸¤å¾—çš„äº‹æƒ…ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080047_step_table = {
		[1] = Task_00080047_step_01,
		[10] = Task_00080047_step_10,
		};

function Task_00080047_step(step)
	if Task_00080047_step_table[step] ~= nil then
		return Task_00080047_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00080047_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080047() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80047) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15089,1);
	if itemNum ~= 0 then
		if itemNum > 15 then
			itemNum = 15;
			package:SetItem(15089, itemNum, 1);
		end
		task:AddTaskStep2(80047, 1, itemNum);
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00080047_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15089,1) < 15 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8927,2,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(80047) then
		return false;
	end

	if IsEquipTypeId(8927) then
		for k = 1, 2 do
			package:AddEquip(8927, 1);
		end
	else 
		package:AddItem(8927,2,1);
	end
	package:DelItemAll(15089,1);

	player:AddExp(14000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00080047_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15089,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80047);
end
