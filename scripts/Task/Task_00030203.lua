--����Ľ�������
function Task_Accept_00030203()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30203) or task:HasCompletedTask(30203) or task:HasSubmitedTask(30203) then
		return false;
	end
	if not task:HasSubmitedTask(30202) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030203()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(30203) or task:HasCompletedTask(30203) or task:HasSubmitedTask(30203) then
		return false;
	end
	if not task:HasSubmitedTask(30202) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00030203()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30203) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030203(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30203) == npcId and Task_Accept_00030203 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30203
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "击败复仇者";
	elseif task:GetTaskSubmitNpc(30203) == npcId then
		if Task_Submit_00030203() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30203
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "击败复仇者";
		elseif task:HasAcceptedTask(30203) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30203
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "击败复仇者";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030203_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我想你已经看到了那些冲过来的天族复仇者，他们已经被仇恨冲昏了头脑，不给他们点厉害，他们不会善罢甘休的。";
	action.m_ActionMsg = "他们很是疯狂。";
	return action;
end

function Task_00030203_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我们一定要给他们点厉害，虽然我很希望你能自己去给他们点教训，可是我担心你的实力没有完全成长，可能会在战斗中受挫，我会帮助你，跟你一起战斗的。";
	action.m_ActionMsg = "天族的人太张狂了，以为自己有多么强大.";
	return action;
end

function Task_00030203_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这下他们总该消停一些了吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030203_step_table = {
		[1] = Task_00030203_step_01,
		[2] = Task_00030203_step_02,
		[10] = Task_00030203_step_10,
		};

function Task_00030203_step(step)
	if Task_00030203_step_table[step] ~= nil then
		return Task_00030203_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030203_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030203() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30203) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00030203_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8911,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30203) then
		return false;
	end

	if IsEquipTypeId(8911) then
		for k = 1, 1 do
			package:AddEquip(8911, 1);
		end
	else 
		package:AddItem(8911,1,1);
	end

	player:AddExp(4000);
	player:getCoin(5500);
	player:getTael(10);
	return true;
end

--��������
function Task_00030203_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30203);
end
