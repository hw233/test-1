--����Ľ�������
function Task_Accept_00061703()
	local player = GetPlayer();
	if player:GetLev() < 63 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61703) or task:HasCompletedTask(61703) or task:HasSubmitedTask(61703) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61702) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61702) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061703()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 63 then
		return false;
	end
	if task:HasAcceptedTask(61703) or task:HasCompletedTask(61703) or task:HasSubmitedTask(61703) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61702) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61702) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061703()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61703) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061703(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61703) == npcId and Task_Accept_00061703 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61703
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "请求宽限";
	elseif task:GetTaskSubmitNpc(61703) == npcId then
		if Task_Submit_00061703() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61703
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "请求宽限";
		elseif task:HasAcceptedTask(61703) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61703
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "请求宽限";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061703_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士真是千恩万谢啊可把我的钱找回来了。";
	action.m_ActionMsg = "怎么才一串铜钱啊，你不是欠了醉鬼一大笔钱的吗？";
	return action;
end

function Task_00061703_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这个……是的，但是我本来是有意还他的，可是我就这么多钱了啊，不然还是请你帮我跟醉鬼请求再宽限几天吧，我这几天会尽力筹钱的，至于这串铜钱，能不能留给我这几天吃饭啊，反正也不够还他的。";
	action.m_ActionMsg = "真是拿你没办法啊。";
	return action;
end

function Task_00061703_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，怎么连你也没办法帮我把钱要回来啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061703_step_table = {
		[1] = Task_00061703_step_01,
		[2] = Task_00061703_step_02,
		[10] = Task_00061703_step_10,
		};

function Task_00061703_step(step)
	if Task_00061703_step_table[step] ~= nil then
		return Task_00061703_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061703_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061703() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61703) then
		return false;
	end
	task:AddTaskStep(61703);
	return true;
end



--�ύ����
function Task_00061703_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(61703) then
		return false;
	end


	player:AddExp(160000);
	player:getCoin(130000);
	return true;
end

--��������
function Task_00061703_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61703);
end
