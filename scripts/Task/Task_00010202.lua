--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00010202()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10202) or task:HasCompletedTask(10202) or task:HasSubmitedTask(10202) then
		return false;
	end
	if not task:HasSubmitedTask(10201) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00010202()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(10202) or task:HasCompletedTask(10202) or task:HasSubmitedTask(10202) then
		return false;
	end
	if not task:HasSubmitedTask(10201) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00010202()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10202) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00010202(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10202) == npcId and Task_Accept_00010202 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10202
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å®‰å…¨çš„æŠ¤é€";
	elseif task:GetTaskAcceptNpc(10202) == npcId and task:HasAcceptedTask(10202) then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10202
		action.m_ActionToken = 3;
		action.m_ActionStep = 11;
		action.m_ActionMsg = "å®‰å…¨çš„æŠ¤é€";
	elseif task:GetTaskSubmitNpc(10202) == npcId then
		if Task_Submit_00010202() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10202
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å®‰å…¨çš„æŠ¤é€";
		elseif task:HasAcceptedTask(10202) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10202
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å®‰å…¨çš„æŠ¤é€";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00010202_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‹‡å£«çœ‹ç€ä½ é¢ç”Ÿå•Šï¼Œæ˜¯ç¬¬ä¸€æ¬¡æ¥å¸®å¿™é€ç”˜éœ²çš„å§ã€‚æˆ‘å¬è¯´ç»éƒ¨å³°è¿™å‘¨å›´æœ€è¿‘ä¸å¤ªå¹³ï¼Œæˆ‘æ€•è·¯ä¸Šä¼šé‡åˆ°å±é™©ï¼Œæ€•æ˜¯è¿˜è¦éº»çƒ¦ä½ æŠ¤é€æˆ‘ä»¬å®‰å…¨å‡ºåŸå•Šã€‚";
	action.m_ActionMsg = "æ²¡é—®é¢˜ï¼Œæˆ‘ä¸€å®šä¼šå°½åŠ›å¸®ä½ ï¼Œæˆ‘ä»¬èµ°å§ã€‚";
	return action;
end

function Task_00010202_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™ä¹ˆå¿«å°±å®Œæˆæˆ‘å©å’çš„äº‹æƒ…äº†å•Šï¼ŒçœŸæ˜¯ä¸€ä¸ªå¾ˆæœ‰æ½œåŠ›çš„å®¶ä¼™ã€‚";
	action.m_ActionMsg = "";
	return action;
end

function Task_00010202_step_11()
	RunConveyAction(GetPlayer(),10202);
end

local Task_00010202_step_table = {
		[1] = Task_00010202_step_01,
		[10] = Task_00010202_step_10,
		[11] = Task_00010202_step_11,
		};

function Task_00010202_step(step)
	if Task_00010202_step_table[step] ~= nil then
		return Task_00010202_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00010202_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010202() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(10202) then
		return false;
	end
	RunConveyAction(player,10202);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00010202_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8917,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(10202) then
		return false;
	end

	if IsEquipTypeId(8917) then
		for k = 1, 1 do
			package:AddEquip(8917, 1);
		end
	else 
		package:AddItem(8917,1,1);
	end

	player:AddExp(1400);
	player:getCoin(1100);
	player:getTael(10);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00010202_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10202);
end
