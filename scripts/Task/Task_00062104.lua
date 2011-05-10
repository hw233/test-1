--����Ľ�������
function Task_Accept_00062104()
	local player = GetPlayer();
	if player:GetLev() < 73 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(62104) or task:HasCompletedTask(62104) or task:HasSubmitedTask(62104) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62103) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62103) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00062104()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 73 then
		return false;
	end
	if task:HasAcceptedTask(62104) or task:HasCompletedTask(62104) or task:HasSubmitedTask(62104) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62103) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62103) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00062104()
	if GetPlayer():GetTaskMgr():HasCompletedTask(62104) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00062104(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(62104) == npcId and Task_Accept_00062104 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 62104
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "物归原主";
	elseif task:GetTaskSubmitNpc(62104) == npcId then
		if Task_Submit_00062104() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62104
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "物归原主";
		elseif task:HasAcceptedTask(62104) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62104
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "物归原主";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00062104_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，在你去解决那些侵扰对我的怪物的时候，我修复了这串佛珠，虽然他很古旧，却依然法力不减，我觉得应该物归原主，虽然奉春华已经修成活佛，他应该还是会需要他曾经的佛珠的。";
	action.m_ActionMsg = "没问题，我去送还给他。";
	return action;
end

function Task_00062104_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "不过他不是普通的活佛，其实他早年是一个受苦受难的农民，可是修行虔诚而笃定，人却少言寡语，一向默默的保佑所有跟他一样的苦难人民。将这佛珠带去给他就好，不需多说什么，活佛其实是什么都知道的。原来活佛是这样的，我知道了。";
	action.m_ActionMsg = "原来活佛是这样的，我知道了。";
	return action;
end

function Task_00062104_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "阿弥陀佛。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00062104_step_table = {
		[1] = Task_00062104_step_01,
		[2] = Task_00062104_step_02,
		[10] = Task_00062104_step_10,
		};

function Task_00062104_step(step)
	if Task_00062104_step_table[step] ~= nil then
		return Task_00062104_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00062104_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00062104() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15018, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(62104) then
		return false;
	end
	task:AddTaskStep(62104);
	return true;
end



--�ύ����
function Task_00062104_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15018,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8930,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(62104) then
		return false;
	end

	if IsEquipTypeId(8930) then
		for k = 1, 1 do
			package:AddEquip(8930, 1);
		end
	else 
		package:AddItem(8930,1,1);
	end
	package:DelItemAll(15018,1);

	player:AddExp(280000);
	player:getCoin(259000);
	return true;
end

--��������
function Task_00062104_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15018,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(62104);
end
