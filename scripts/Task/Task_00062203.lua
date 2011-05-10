--����Ľ�������
function Task_Accept_00062203()
	local player = GetPlayer();
	if player:GetLev() < 73 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(62203) or task:HasCompletedTask(62203) or task:HasSubmitedTask(62203) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62202) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62202) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00062203()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 73 then
		return false;
	end
	if task:HasAcceptedTask(62203) or task:HasCompletedTask(62203) or task:HasSubmitedTask(62203) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62202) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62202) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00062203()
	if GetPlayer():GetTaskMgr():HasCompletedTask(62203) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00062203(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(62203) == npcId and Task_Accept_00062203 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 62203
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "水落石出";
	elseif task:GetTaskSubmitNpc(62203) == npcId then
		if Task_Submit_00062203() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62203
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "水落石出";
		elseif task:HasAcceptedTask(62203) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62203
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "水落石出";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00062203_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "施主居然已经见过冷锋夜和无暇仙子了，我想也许你猜的到无暇仙子的确就是冷姑娘，可是我却一直瞒着她从没有跟说过她身世的事情，其实都是为了保护冷姑娘，当初冷家受害，我拼死救出年幼的冷姑娘，虽然她不记事，却已经被当时的事情吓坏了，而且心情抑郁了很久，现在好不容易才康复的。";
	action.m_ActionMsg = "原来冷锋夜的怀疑是真的。";
	return action;
end

function Task_00062203_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "也许他不想让冷姑娘知道她的真实身世，是为了不再让她回忆起曾经经历的痛苦吧。毕竟现在形势险恶，有他共同对付夜摩盟也不是坏事，况且海波流最近又出现了一些绝境冰妖，威胁到了我和无暇仙子的安全。";
	action.m_ActionMsg = "原来是这样，那我去解决掉他们，决不能再让冷姑娘遇到威胁。";
	return action;
end

function Task_00062203_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "施主保重，后会有期。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00062203_step_table = {
		[1] = Task_00062203_step_01,
		[2] = Task_00062203_step_02,
		[10] = Task_00062203_step_10,
		};

function Task_00062203_step(step)
	if Task_00062203_step_table[step] ~= nil then
		return Task_00062203_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00062203_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00062203() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(62203) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00062203_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8930,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(62203) then
		return false;
	end

	if IsEquipTypeId(8930) then
		for k = 1, 1 do
			package:AddEquip(8930, 1);
		end
	else 
		package:AddItem(8930,1,1);
	end

	player:AddExp(280000);
	player:getCoin(259000);
	return true;
end

--��������
function Task_00062203_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(62203);
end
