--����Ľ�������
function Task_Accept_00020405()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 22 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20405) or task:HasCompletedTask(20405) or task:HasSubmitedTask(20405) then
		return false;
	end
	if not task:HasSubmitedTask(20404) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020405()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 22 then
		return false;
	end
	if task:HasAcceptedTask(20405) or task:HasCompletedTask(20405) or task:HasSubmitedTask(20405) then
		return false;
	end
	if not task:HasSubmitedTask(20404) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020405()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20405) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020405(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20405) == npcId and Task_Accept_00020405 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20405
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "询问使节";
	elseif task:GetTaskSubmitNpc(20405) == npcId then
		if Task_Submit_00020405() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20405
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "询问使节";
		elseif task:HasAcceptedTask(20405) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20405
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "询问使节";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020405_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你是为甘露的事情来的？这次的甘露有问题吗？";
	action.m_ActionMsg = "是的，难陀罗大龙王说甘露的数量只有平时的一半。";
	return action;
end

function Task_00020405_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "怎么会这样！我去天庭取甘露的时候什么事情也没有啊，难道是帝释天开始反悔了？";
	action.m_ActionMsg = "看来你也不知道是怎么回事了。";
	return action;
end

function Task_00020405_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "使节告诉你这是怎么回事了吗？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020405_step_table = {
		[1] = Task_00020405_step_01,
		[2] = Task_00020405_step_02,
		[10] = Task_00020405_step_10,
		};

function Task_00020405_step(step)
	if Task_00020405_step_table[step] ~= nil then
		return Task_00020405_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020405_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020405() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20405) then
		return false;
	end
	task:AddTaskStep(20405);
	return true;
end



--�ύ����
function Task_00020405_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(20405) then
		return false;
	end


	player:AddExp(2200);
	player:getCoin(3700);
	return true;
end

--��������
function Task_00020405_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20405);
end
