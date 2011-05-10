--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00032004()
	local player = GetPlayer();
	if player:GetLev() < 75 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(32004) or task:HasCompletedTask(32004) or task:HasSubmitedTask(32004) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(32003) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(32003) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00032004()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 75 then
		return false;
	end
	if task:HasAcceptedTask(32004) or task:HasCompletedTask(32004) or task:HasSubmitedTask(32004) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(32003) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(32003) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00032004()
	if GetPlayer():GetTaskMgr():HasCompletedTask(32004) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00032004(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(32004) == npcId and Task_Accept_00032004 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 32004
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å‘èµ·æŒ‘æˆ˜";
	elseif task:GetTaskSubmitNpc(32004) == npcId then
		if Task_Submit_00032004() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 32004
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å‘èµ·æŒ‘æˆ˜";
		elseif task:HasAcceptedTask(32004) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 32004
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å‘èµ·æŒ‘æˆ˜";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00032004_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å‹‡å£«ä½ åšçš„å¾ˆå¥½ï¼Œç°åœ¨æ˜¯è¦è®©å¤œæ‘©ç›ŸçŸ¥é“æˆ‘ä»¬çš„å‰å®³çš„æ—¶å€™äº†ã€‚æŠŠç°å²©å®ˆå«çš„å¤´é¢…æŒ‚åˆ°å¹»æµ·åŸçš„æ™¨é’Ÿé•‡ä¸Šå»ï¼Œå°±ä¼šè®©ä»é‚£é‡Œç»è¿‡çš„å¤œæ‘©ç›Ÿå…¨éƒ½çœ‹åˆ°ï¼Œæˆ‘ä»¬å·²ç»å‘ä»–ä»¬å‘èµ·æŒ‘æˆ˜äº†ã€‚";
	action.m_ActionMsg = "æˆ‘å¾ˆä¹æ„æ•ˆåŠ³ï¼Œä¹Ÿè®©é‚£å¸®å®¶ä¼™å°å°ç—›è‹¦çš„æ»‹å‘³ã€‚";
	return action;
end

function Task_00032004_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç°åœ¨å°±æ˜¯æˆ‘ä»¬è¿›å…¥å…¨é¢å¤‡æˆ˜çŠ¶æ€çš„æ—¶åˆ»äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00032004_step_table = {
		[1] = Task_00032004_step_01,
		[10] = Task_00032004_step_10,
		};

function Task_00032004_step(step)
	if Task_00032004_step_table[step] ~= nil then
		return Task_00032004_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00032004_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00032004() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15083, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(32004) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00032004_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15083,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(32004) then
		return false;
	end

	package:DelItemAll(15083,1);

	player:AddExp(300000);
	player:getCoin(300000);
	player:getTael(40);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00032004_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15083,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(32004);
end
