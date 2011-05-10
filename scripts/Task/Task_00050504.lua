--����Ľ�������
function Task_Accept_00050504()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50504) or task:HasCompletedTask(50504) or task:HasSubmitedTask(50504) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050504()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(50504) or task:HasCompletedTask(50504) or task:HasSubmitedTask(50504) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050504()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50504) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050504(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50504) == npcId and Task_Accept_00050504 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50504
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "王的奖赏";
	elseif task:GetTaskSubmitNpc(50504) == npcId then
		if Task_Submit_00050504() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50504
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "王的奖赏";
		elseif task:HasAcceptedTask(50504) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50504
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "王的奖赏";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050504_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "恩，你最近进步很快，我很高兴，不愧是我们魔族的勇士。";
	action.m_ActionMsg = "您过奖了，我会努力训练自己，争取早日为魔族效力。";
	return action;
end

function Task_00050504_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我让铁匠铺的强化大师给你做了一把宝剑，作为奖赏赐予你，现在你就去拿吧，然后让铁匠铺的大师们帮你把装备进行强化，这样你会变得更强大。";
	action.m_ActionMsg = "多谢您的赏赐，我这就去。";
	return action;
end

function Task_00050504_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，我等你很久了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050504_step_table = {
		[1] = Task_00050504_step_01,
		[2] = Task_00050504_step_02,
		[10] = Task_00050504_step_10,
		};

function Task_00050504_step(step)
	if Task_00050504_step_table[step] ~= nil then
		return Task_00050504_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050504_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050504() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50504) then
		return false;
	end
	task:AddTaskStep(50504);
	return true;
end



--�ύ����
function Task_00050504_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(368,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(50504) then
		return false;
	end

	if IsEquipTypeId(368) then
		for k = 1, 1 do
			package:AddEquip(368, 1);
		end
	else 
		package:AddItem(368,1,1);
	end

	player:AddExp(2000);
	player:getCoin(10000);
	return true;
end

--��������
function Task_00050504_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50504);
end
