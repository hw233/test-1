--����Ľ�������
function Task_Accept_00020102()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 5 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20102) or task:HasCompletedTask(20102) or task:HasSubmitedTask(20102) then
		return false;
	end
	if not task:HasSubmitedTask(20101) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020102()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 5 then
		return false;
	end
	if task:HasAcceptedTask(20102) or task:HasCompletedTask(20102) or task:HasSubmitedTask(20102) then
		return false;
	end
	if not task:HasSubmitedTask(20101) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020102()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20102) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020102(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20102) == npcId and Task_Accept_00020102 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20102
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "拜见张良";
	elseif task:GetTaskSubmitNpc(20102) == npcId then
		if Task_Submit_00020102() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20102
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "拜见张良";
		elseif task:HasAcceptedTask(20102) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20102
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "拜见张良";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020102_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士"..GetPlayerName(GetPlayer()).."，很高兴你选择成为魔族的一员，我们魔族可是一个勇敢而强大的种族，我将带领你参加对抗罗阅王的影子的战斗，让你感受一下魔族战斗的强势，你会为你的选择感到自豪。";
	action.m_ActionMsg = "我很期待跟你的战斗，开始吧。";
	return action;
end

function Task_00020102_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "战斗结束你就可以去拜见我们的魔族之王阿修罗，他就在旁边的光明城里，去向他说明你对魔族的忠心吧！";
	action.m_ActionMsg = "那就让我们比试一下吧。";
	return action;
end

function Task_00020102_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "经过这场战斗，你已经感受到了战斗的魅力和我们魔族的气势了吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020102_step_table = {
		[1] = Task_00020102_step_01,
		[2] = Task_00020102_step_02,
		[10] = Task_00020102_step_10,
		};

function Task_00020102_step(step)
	if Task_00020102_step_table[step] ~= nil then
		return Task_00020102_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020102_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020102() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20102) then
		return false;
	end
	RunAutoBattleAction(player,6145,100);
	task:AddTaskStep(20102);

	return true;
end



--�ύ����
function Task_00020102_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(2,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(20102) then
		return false;
	end

	if IsEquipTypeId(2) then
		for k = 1, 1 do
			package:AddEquip(2, 1);
		end
	else 
		package:AddItem(2,1,1);
	end

	player:AddExp(650);
	player:getCoin(550);
	return true;
end

--��������
function Task_00020102_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20102);
end
