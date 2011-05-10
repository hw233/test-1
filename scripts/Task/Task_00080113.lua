--任务的接受条件
function Task_Accept_00080113()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80113) or task:HasCompletedTask(80113) or (task:HasSubmitedTask(80113) and (GetSharpDay(task:GetTaskEndTime(80113)) == GetSharpDay(os.time()))) then 
		return false;
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00080113()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(80113) or task:HasCompletedTask(80113) or (task:HasSubmitedTask(80113) and (GetSharpDay(task:GetTaskEndTime(80113)) == GetSharpDay(os.time()))) then 
		return false;
	end
	return true;
end


--任务完成条件
function Task_Submit_00080113()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80113) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00080113(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80113) == npcId and Task_Accept_00080113 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80113
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "瀹楁棌蹇呰儨";
	elseif task:GetTaskSubmitNpc(80113) == npcId then
		if Task_Submit_00080113() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80113
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "瀹楁棌蹇呰儨";
		elseif task:HasAcceptedTask(80113) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80113
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "瀹楁棌蹇呰儨";
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00080113_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "鍦ㄨ繖鍧楀北娴峰ぇ闄嗕笂锛屽闆ㄥ悗鏄ョ瑡鑸秾鐜颁簡璁稿瀹楁棌锛岀浉浜掓帬澶哄氨鏄綘浠殑瀹垮懡锛屽敮鏈夎儨鍒╂墠鏄洰鏍囷紒";
	action.m_ActionMsg = "鍦ㄦ垜鐪间腑锛屽彧鏈夎儨鍒╂病鏈夊け璐ワ紒";
	return action;
end

function Task_00080113_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "鑳滃埄灞炰簬浣狅紒";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080113_step_table = {
		[1] = Task_00080113_step_01,
		[10] = Task_00080113_step_10,
		};

function Task_00080113_step(step)
	if Task_00080113_step_table[step] ~= nil then
		return Task_00080113_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00080113_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080113() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80113) then
		return false;
	end
	return true;
end



--提交任务
function Task_00080113_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80113) then
		return false;
	end


	player:AddExp(150000);
	player:getCoin(10000);
	return true;
end

--放弃任务
function Task_00080113_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80113);
end
