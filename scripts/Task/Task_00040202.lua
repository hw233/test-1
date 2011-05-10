--����Ľ�������
function Task_Accept_00040202()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40202) or task:HasCompletedTask(40202) or task:HasSubmitedTask(40202) then
		return false;
	end
	if not task:HasSubmitedTask(40201) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040202()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(40202) or task:HasCompletedTask(40202) or task:HasSubmitedTask(40202) then
		return false;
	end
	if not task:HasSubmitedTask(40201) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040202()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40202) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040202(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40202) == npcId and Task_Accept_00040202 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40202
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "意外";
	elseif task:GetTaskSubmitNpc(40202) == npcId then
		if Task_Submit_00040202() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40202
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "意外";
		elseif task:HasAcceptedTask(40202) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40202
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "意外";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040202_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "悦意王妃平时对我们的国事可是从来都不闻不问的，这次是怎么了，居然主动表示出关心的态度了，我觉得她一定是有什么计谋了。";
	action.m_ActionMsg = "这个我就不清楚了，我只是负责传话的。";
	return action;
end

function Task_00040202_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "不过你也不要让她知道我的怀疑态度，就回去告诉她，我们这里一切都很正常，甘露的供应也没有问题。不过我是要计划一下了，我会派人暗中察看一下王妃的行踪。";
	action.m_ActionMsg = "好吧，我回去跟她说。";
	return action;
end

function Task_00040202_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我知道了，谢谢你勇士。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040202_step_table = {
		[1] = Task_00040202_step_01,
		[2] = Task_00040202_step_02,
		[10] = Task_00040202_step_10,
		};

function Task_00040202_step(step)
	if Task_00040202_step_table[step] ~= nil then
		return Task_00040202_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040202_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040202() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40202) then
		return false;
	end
	task:AddTaskStep(40202);
	return true;
end



--�ύ����
function Task_00040202_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8908,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(40202) then
		return false;
	end

	if IsEquipTypeId(8908) then
		for k = 1, 1 do
			package:AddEquip(8908, 1);
		end
	else 
		package:AddItem(8908,1,1);
	end

	player:AddExp(1100);
	player:getCoin(800);
	return true;
end

--��������
function Task_00040202_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40202);
end
