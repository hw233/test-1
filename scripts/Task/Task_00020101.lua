--����Ľ�������
function Task_Accept_00020101()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20101) or task:HasCompletedTask(20101) or task:HasSubmitedTask(20101) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020101()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(20101) or task:HasCompletedTask(20101) or task:HasSubmitedTask(20101) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020101()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20101) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020101(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20101) == npcId and Task_Accept_00020101 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20101
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "初到地界";
	elseif task:GetTaskSubmitNpc(20101) == npcId then
		if Task_Submit_00020101() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20101
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "初到地界";
		elseif task:HasAcceptedTask(20101) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20101
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "初到地界";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020101_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你好年轻的勇士，欢迎你来到地界。很快你将遇到各种不同的任务和挑战，可以点击屏幕下方的“任务按钮”，查看可接任务 和当前任务，以及详细的任务描述和任务流程。";
	action.m_ActionMsg = "我明白了，还有吗？";
	return action;
end

function Task_00020101_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "当前任务有任务目标描述，可以帮助你更有效的完成你的任务，点击任务地点名称或者人物名字，你会自动走向目标的，现在 你就去拜访我们魔族的名将张良吧。";
	action.m_ActionMsg = "原来可以自动寻路啊，感谢你对我的指导。";
	return action;
end

function Task_00020101_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "魔族欢迎你，勇士。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020101_step_table = {
		[1] = Task_00020101_step_01,
		[2] = Task_00020101_step_02,
		[10] = Task_00020101_step_10,
		};

function Task_00020101_step(step)
	if Task_00020101_step_table[step] ~= nil then
		return Task_00020101_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020101_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020101() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20101) then
		return false;
	end
	task:AddTaskStep(20101);
	return true;
end



--�ύ����
function Task_00020101_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(1,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(20101) then
		return false;
	end

	if IsEquipTypeId(1) then
		for k = 1, 1 do
			package:AddEquip(1, 1);
		end
	else 
		package:AddItem(1,1,1);
	end

	player:AddExp(650);
	player:getCoin(500);
	return true;
end

--��������
function Task_00020101_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20101);
end
