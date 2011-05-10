--����Ľ�������
function Task_Accept_00030607()
	local player = GetPlayer();
	if player:GetLev() < 45 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30607) or task:HasCompletedTask(30607) or task:HasSubmitedTask(30607) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30606) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30606) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030607()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 45 then
		return false;
	end
	if task:HasAcceptedTask(30607) or task:HasCompletedTask(30607) or task:HasSubmitedTask(30607) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30606) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30606) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030607()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30607) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030607(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30607) == npcId and Task_Accept_00030607 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30607
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "恢复意识";
	elseif task:GetTaskSubmitNpc(30607) == npcId then
		if Task_Submit_00030607() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30607
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "恢复意识";
		elseif task:HasAcceptedTask(30607) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30607
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "恢复意识";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030607_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你现在好点了吗，想起来发生什么事情了吗？我想你一定是中了夜摩盟那些小鬼的毒了，最近不仅是你，好多人都发生过同样的事情，这一切都是夜叉王的阴谋。";
	action.m_ActionMsg = "好像就是这样的。";
	return action;
end

function Task_00030607_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你一定要记得，在山海大陆，有我们的朋友，但是也有更多的敌人，不要轻易相信别人。现在请你帮我将我们最近收集到的关于夜叉王的阴谋的情报送给驻守摩加古镇的大将羽月吧。\n";
	action.m_ActionMsg = "好的，那我去拜见羽月了。";
	return action;
end

function Task_00030607_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你就是那传说中影响遍布山海大陆的勇士吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030607_step_table = {
		[1] = Task_00030607_step_01,
		[2] = Task_00030607_step_02,
		[10] = Task_00030607_step_10,
		};

function Task_00030607_step(step)
	if Task_00030607_step_table[step] ~= nil then
		return Task_00030607_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030607_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030607() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15045, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(30607) then
		return false;
	end
	task:AddTaskStep(30607);
	return true;
end



--�ύ����
function Task_00030607_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15045,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8933,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30607) then
		return false;
	end

	if IsEquipTypeId(8933) then
		for k = 1, 1 do
			package:AddEquip(8933, 1);
		end
	else 
		package:AddItem(8933,1,1);
	end
	package:DelItemAll(15045,1);

	player:AddExp(36000);
	player:getCoin(25000);
	player:getTael(40);
	return true;
end

--��������
function Task_00030607_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15045,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30607);
end
