--����Ľ�������
function Task_Accept_00060401()
	local player = GetPlayer();
	if player:GetLev() < 36 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60401) or task:HasCompletedTask(60401) or task:HasSubmitedTask(60401) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060401()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 36 then
		return false;
	end
	if task:HasAcceptedTask(60401) or task:HasCompletedTask(60401) or task:HasSubmitedTask(60401) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00060401()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60401) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060401(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60401) == npcId and Task_Accept_00060401 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60401
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "狼魔首领";
	elseif task:GetTaskSubmitNpc(60401) == npcId then
		if Task_Submit_00060401() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60401
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "狼魔首领";
		elseif task:HasAcceptedTask(60401) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60401
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "狼魔首领";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060401_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，还在持续修炼的路吗？我正好有一些艰难的任务，不知道你是否愿意尝试一下啊，如果你能完成，说明你的修炼很有成效。";
	action.m_ActionMsg = "原来是想考验我啊，当然没有问题。";
	return action;
end

function Task_00060401_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "在离这里东北方的黑风岗来了一个狼魔首领，这个家伙可不是一般的厉害，是那些普通的狼族不能比的，虽然我知道你胆气很足，但是还是不得不嘱咐你小心行事。";
	action.m_ActionMsg = "有这么厉害？别吓我啊，我去看看。";
	return action;
end

function Task_00060401_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "怎么样啊勇士，这个考验没有那么简单吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060401_step_table = {
		[1] = Task_00060401_step_01,
		[2] = Task_00060401_step_02,
		[10] = Task_00060401_step_10,
		};

function Task_00060401_step(step)
	if Task_00060401_step_table[step] ~= nil then
		return Task_00060401_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060401_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060401() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60401) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00060401_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(1301,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(60401) then
		return false;
	end

	if IsEquipTypeId(1301) then
		for k = 1, 1 do
			package:AddEquip(1301, 1);
		end
	else 
		package:AddItem(1301,1,1);
	end

	player:AddExp(5500);
	player:getCoin(8000);
	return true;
end

--��������
function Task_00060401_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60401);
end
