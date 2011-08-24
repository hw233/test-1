require("global")

function RunGreatFighterAction(fgtId)
	local MsgTable = {};
	
	local player = GetPlayer();
	local playerLev = player:GetLev();
	local vipLev = player:getVipLevel();
	local fighterLev = GetGreatFighterLev(fgtId);
	local color = GetGreatFighterCorlor(fgtId);
	local submitCnt = player:getGreatFighterFavorSubmitCount(fgtId);
	local maxSubmitCnt = 10;
	if color >= 4 then
		if vipLev >= 7 then
			maxSubmitCnt = 30;
		elseif vipLev >= 5 then
			maxSubmitCnt = 20;
		end			
	end
	if submitCnt >= maxSubmitCnt then
		table.insert(MsgTable, RunSepcialCollectAction1());
	elseif playerLev + 15 >= fighterLev then
		table.insert(MsgTable, RunSepcialCollectAction2(fgtId));
	else
		table.insert(MsgTable, RunSepcialCollectAction3());
	end
	if player:getGreatFighterFriendliness(fgtId) >= GetGreatFighterReqFriendliness(fgtId) and playerLev + 10 >= fighterLev then
		table.insert(MsgTable, RunGreatCtrlAction(fgtId));
	end
	-- return MsgTable; TODO:
	return {};
end

function RunSepcialCollectAction1()
	local action = ActionTable:Instance();
	
	action.m_ActionType = 0x0001;
	action.m_ActionID = 0;
	action.m_ActionToken = 255;
	action.m_ActionStep = 0;
	action.m_ActionMsg =  "今天散仙喜好品赠送额度已满";
	return action;	
end

function RunSepcialCollectAction2(fgtId)
	local action = ActionTable:Instance();
	
	action.m_ActionType = 0x050;
	action.m_ActionID = fgtId;
	action.m_ActionToken = 5;
	action.m_ActionStep = 01;
	action.m_ActionMsg =  "[1:" .. fgtId .. "]喜好品任务";
	return action;	
end

function RunSepcialCollectAction3()
	local action = ActionTable:Instance();
	
	action.m_ActionType = 0x0001;
	action.m_ActionID = 0;
	action.m_ActionToken = 255;
	action.m_ActionStep = 0;
	action.m_ActionMsg =  "赠送散仙喜好品需要主将等级与散仙等级在15级之内";
	return action;	
end


function RunGreatCollectActionStep(fgtId)
	local action = ActionTable:Instance();
	local player = GetPlayer();
	local vipLev = player:getVipLevel();
	local color = GetGreatFighterCorlor(fgtId);
	action.m_ActionType = 0x050;
	action.m_ActionID = fgtId;
	action.m_ActionToken = 5;
	action.m_ActionStep = 0;
	if vipLev >= 7 and color >= 4 then
		action.m_ActionMsg = "[4:" .. GetPlayer():getGreatFighterFavor(fgtId) .. "]" .. "x4  " .. "(今天第" .. GetPlayer():getGreatFighterFavorSubmitCount(fgtId)+1 .. "/30次交付)";
	elseif vipLev >= 5 and color >= 4 then
		action.m_ActionMsg = "[4:" .. GetPlayer():getGreatFighterFavor(fgtId) .. "]" .. "x4  " .. "(今天第" .. GetPlayer():getGreatFighterFavorSubmitCount(fgtId)+1 .. "/20次交付)";
	else
		action.m_ActionMsg = "[4:" .. GetPlayer():getGreatFighterFavor(fgtId) .. "]" .. "x4  " .. "(今天第" .. GetPlayer():getGreatFighterFavorSubmitCount(fgtId)+1 .. "/10次交付)";
	end
	return action;	
end

function RunGreatCtrlAction(fgtId)
	local action = ActionTable:Instance();
	
	action.m_ActionType = 0x051;
	action.m_ActionID = fgtId;
	action.m_ActionToken = 5;
	action.m_ActionStep = 01;
	action.m_ActionMsg = "[1:" .. fgtId .. "]收服洗礼";
	
	return action;		
end

function RunGreatCtrlActionStep(fgtId)
	local action = ActionTable:Instance();
	
	action.m_ActionType = 0x051;
	action.m_ActionID = fgtId;
	action.m_ActionToken = 5;
	action.m_ActionStep = 0
	action.m_ActionMsg = "勇士，经过这些日子的相处，你已经逐渐赢得了我对你的好感和信任，并且让我相信能够很好的与你为伍。但是在这之前，你还要最后接受一次挑战，如果你能够战胜我，我才会心甘情愿的追随你。";
	
	return action;		
end

function RunGreatTaskAction(fgtId)
	local player = GetPlayer();
	local taskId;
	if GetPlayerData(6) == 0 then
		taskId = 80001;
	else
		taskId = 80002;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(taskId) then
		task:AddTaskStep(taskId);
		return true;
	end
	return false;
end

function RunTakeFighterAction()
	local player = GetPlayer();
	local taskMgr = player:GetTaskMgr();
	local taskId;
	local update = false;
	local fgtCnt = player:getFighterCount();
	if GetPlayerData(6) == 0 then
		if taskMgr:HasAcceptedTask(80003) then
			taskId = 80003;
			if fgtCnt > 1 then
				update = true;
			end
		elseif taskMgr:HasAcceptedTask(80004) then
			taskId = 80004;
			if fgtCnt > 2 then
				update = true;
			end
		else
			return;
		end
	else
		if taskMgr:HasAcceptedTask(80005) then
			taskId = 80005;
			if fgtCnt > 1 then
				update = true;
			end
		elseif taskMgr:HasAcceptedTask(80006) then
			taskId = 80006;
			if fgtCnt > 2 then
				update = true;
			end
		else
			return;
		end		
	end
	if update then
		taskMgr:AddTaskStep(taskId);
	end
end
