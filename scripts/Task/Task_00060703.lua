--����Ľ�������
function Task_Accept_00060703()
	local player = GetPlayer();
	if player:GetLev() < 43 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60703) or task:HasCompletedTask(60703) or task:HasSubmitedTask(60703) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60702) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60702) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060703()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 43 then
		return false;
	end
	if task:HasAcceptedTask(60703) or task:HasCompletedTask(60703) or task:HasSubmitedTask(60703) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60702) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60702) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00060703()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60703) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060703(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60703) == npcId and Task_Accept_00060703 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60703
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "失语之症";
	elseif task:GetTaskSubmitNpc(60703) == npcId then
		if Task_Submit_00060703() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60703
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "失语之症";
		elseif task:HasAcceptedTask(60703) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60703
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "失语之症";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060703_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，我已经说过那样的话了，所以我也不想反悔，我只想告诉你，紫衣是因为患了失语症，没办法告诉你她的选择，但实际上，她跟我在一起很幸福。";
	action.m_ActionMsg = "那你想怎么办呢？";
	return action;
end

function Task_00060703_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我已经被你打败了，所以争不过你，可是天武如果真心的想得到紫衣姑娘，我只有一个条件，只要他能治好紫衣姑娘的失语症，我便不再阻碍他的想法。";
	action.m_ActionMsg = "原来是这样，那我去找天武说一下。";
	return action;
end

function Task_00060703_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "盛一刀果然不肯轻易对紫衣放手啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060703_step_table = {
		[1] = Task_00060703_step_01,
		[2] = Task_00060703_step_02,
		[10] = Task_00060703_step_10,
		};

function Task_00060703_step(step)
	if Task_00060703_step_table[step] ~= nil then
		return Task_00060703_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060703_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060703() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60703) then
		return false;
	end
	task:AddTaskStep(60703);
	return true;
end



--�ύ����
function Task_00060703_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8914,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(60703) then
		return false;
	end

	if IsEquipTypeId(8914) then
		for k = 1, 1 do
			package:AddEquip(8914, 1);
		end
	else 
		package:AddItem(8914,1,1);
	end

	player:AddExp(25000);
	player:getCoin(19000);
	return true;
end

--��������
function Task_00060703_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60703);
end
