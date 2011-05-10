--����Ľ�������
function Task_Accept_00030803()
	local player = GetPlayer();
	if player:GetLev() < 50 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30803) or task:HasCompletedTask(30803) or task:HasSubmitedTask(30803) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30802) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30802) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030803()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 50 then
		return false;
	end
	if task:HasAcceptedTask(30803) or task:HasCompletedTask(30803) or task:HasSubmitedTask(30803) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30802) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30802) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030803()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30803) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030803(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30803) == npcId and Task_Accept_00030803 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30803
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "就地取材";
	elseif task:GetTaskSubmitNpc(30803) == npcId then
		if Task_Submit_00030803() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30803
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "就地取材";
		elseif task:HasAcceptedTask(30803) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30803
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "就地取材";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030803_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士对于你的倾力相助，我代表整个镇邪派表示感谢，以后若有需要我们的地方，定当鼎力相助，现在只要再去响石滩的碎石阵，采一块尘封火岩，打造装备的材料就够了，带着这些材料去找我旁边的赤佬温吧，他就是愿意帮助我们的名将。";
	action.m_ActionMsg = "希望他的实力是我们所需要的，那才是不枉我的苦心。";
	return action;
end

function Task_00030803_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "真是难为祖言帮主了，这么恶劣的环境中还在为我准备这么好的装备材料。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030803_step_table = {
		[1] = Task_00030803_step_01,
		[10] = Task_00030803_step_10,
		};

function Task_00030803_step(step)
	if Task_00030803_step_table[step] ~= nil then
		return Task_00030803_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030803_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030803() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30803) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15063,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15063, itemNum, 1);
		end
		task:AddTaskStep2(30803, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00030803_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15063,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(30803) then
		return false;
	end

	package:DelItemAll(15063,1);

	player:AddExp(56000);
	player:getCoin(43500);
	player:getTael(20);
	return true;
end

--��������
function Task_00030803_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15063,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30803);
end
