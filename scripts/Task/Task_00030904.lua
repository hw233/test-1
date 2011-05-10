--����Ľ�������
function Task_Accept_00030904()
	local player = GetPlayer();
	if player:GetLev() < 55 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30904) or task:HasCompletedTask(30904) or task:HasSubmitedTask(30904) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30903) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30903) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030904()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 55 then
		return false;
	end
	if task:HasAcceptedTask(30904) or task:HasCompletedTask(30904) or task:HasSubmitedTask(30904) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30903) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30903) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030904()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30904) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030904(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30904) == npcId and Task_Accept_00030904 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30904
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "审问摩伽";
	elseif task:GetTaskSubmitNpc(30904) == npcId then
		if Task_Submit_00030904() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30904
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "审问摩伽";
		elseif task:HasAcceptedTask(30904) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30904
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "审问摩伽";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030904_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "看来狡猾的摩可拿在我们发现他之前，就已经逃走了。";
	action.m_ActionMsg = "我想是的。";
	return action;
end

function Task_00030904_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "但是那个摩伽肯定不是什么好东西，你现在去审问他，一定不能轻易放过。";
	action.m_ActionMsg = "哼哼，他要是敢不说实话，我砍下他的舌头。";
	return action;
end

function Task_00030904_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "怎么又有生人来找我了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030904_step_table = {
		[1] = Task_00030904_step_01,
		[2] = Task_00030904_step_02,
		[10] = Task_00030904_step_10,
		};

function Task_00030904_step(step)
	if Task_00030904_step_table[step] ~= nil then
		return Task_00030904_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030904_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030904() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30904) then
		return false;
	end
	task:AddTaskStep(30904);
	return true;
end



--�ύ����
function Task_00030904_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(505,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30904) then
		return false;
	end

	if IsEquipTypeId(505) then
		for k = 1, 1 do
			package:AddEquip(505, 1);
		end
	else 
		package:AddItem(505,1,1);
	end

	player:AddExp(76000);
	player:getCoin(70000);
	player:getTael(20);
	return true;
end

--��������
function Task_00030904_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30904);
end
