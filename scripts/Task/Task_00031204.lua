--����Ľ�������
function Task_Accept_00031204()
	local player = GetPlayer();
	if player:GetLev() < 59 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31204) or task:HasCompletedTask(31204) or task:HasSubmitedTask(31204) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31203) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31203) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031204()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 59 then
		return false;
	end
	if task:HasAcceptedTask(31204) or task:HasCompletedTask(31204) or task:HasSubmitedTask(31204) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31203) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31203) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031204()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31204) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031204(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31204) == npcId and Task_Accept_00031204 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31204
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "寻找雪莲";
	elseif task:GetTaskSubmitNpc(31204) == npcId then
		if Task_Submit_00031204() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31204
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "寻找雪莲";
		elseif task:HasAcceptedTask(31204) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31204
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "寻找雪莲";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031204_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士我看到你可是负着重伤回来的啊，是不是遇到摩睺了。摩睺这家伙应该是夜摩盟的，我也怀疑他很久了，自从他背叛了魔族逃到这山海大陆，就暗暗的在密谋一些什么东西。他曾是掌握了黑暗魔法的人，在贪欲的驱使下背叛了阿修罗。如果不借助能反抗这种魔法的力量，你很难恢复体力，但我仍为你的勇气而感动。";
	action.m_ActionMsg = "我正是来找您帮忙的，我似乎受了摩睺的法力的控制。";
	return action;
end

function Task_00031204_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "现在你想要回复体力，只能去雪压峰采一株醒神雪莲，这种雪莲难得一见，却是对抗黑暗魔法最好的东西，你去碰一下运气吧。";
	action.m_ActionMsg = "太好了，我去找找看。";
	return action;
end

function Task_00031204_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我就相信你不是一般的人，果然有超凡的运气啊，这么珍贵的雪莲都能被你找到。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031204_step_table = {
		[1] = Task_00031204_step_01,
		[2] = Task_00031204_step_02,
		[10] = Task_00031204_step_10,
		};

function Task_00031204_step(step)
	if Task_00031204_step_table[step] ~= nil then
		return Task_00031204_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031204_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031204() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31204) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15067,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15067, itemNum, 1);
		end
		task:AddTaskStep2(31204, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00031204_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15067,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8915,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31204) then
		return false;
	end

	if IsEquipTypeId(8915) then
		for k = 1, 1 do
			package:AddEquip(8915, 1);
		end
	else 
		package:AddItem(8915,1,1);
	end
	package:DelItemAll(15067,1);

	player:AddExp(100000);
	player:getCoin(100000);
	player:getTael(30);
	return true;
end

--��������
function Task_00031204_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15067,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31204);
end
