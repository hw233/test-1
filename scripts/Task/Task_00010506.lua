--����Ľ�������
function Task_Accept_00010506()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 25 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10506) or task:HasCompletedTask(10506) or task:HasSubmitedTask(10506) then
		return false;
	end
	if not task:HasSubmitedTask(10505) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010506()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 25 then
		return false;
	end
	if task:HasAcceptedTask(10506) or task:HasCompletedTask(10506) or task:HasSubmitedTask(10506) then
		return false;
	end
	if not task:HasSubmitedTask(10505) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010506()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10506) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010506(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10506) == npcId and Task_Accept_00010506 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10506
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "前往山海大陆";
	elseif task:GetTaskSubmitNpc(10506) == npcId then
		if Task_Submit_00010506() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10506
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "前往山海大陆";
		elseif task:HasAcceptedTask(10506) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10506
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "前往山海大陆";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010506_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "夜叉王和摩可拿还真是狡猾，明明发现他们的踪迹了，却又逃跑了。不过能够消灭侵袭到这儿的狼鬼族势力，你已经做的很好了。";
	action.m_ActionMsg = "收拾这群狼鬼族对我来说还是很简单的。";
	return action;
end

function Task_00010506_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我很欣赏你的精神，我想你已经见识到我们正面对的仇恨和威胁，你的实力需要增强，你的战斗队伍需要扩大，从这儿的出口出去，到那片充满挑战的山海大陆去吧，迎接新的挑战，寻找更多的名将，位于阿兰若古城的天族前线营地的士兵会接待你的。";
	action.m_ActionMsg = "我很希望接受新的挑战。";
	return action;
end

function Task_00010506_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "初来山海大陆的天族勇士，欢迎你。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010506_step_table = {
		[1] = Task_00010506_step_01,
		[2] = Task_00010506_step_02,
		[10] = Task_00010506_step_10,
		};

function Task_00010506_step(step)
	if Task_00010506_step_table[step] ~= nil then
		return Task_00010506_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010506_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010506() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(10506) then
		return false;
	end
	task:AddTaskStep(10506);
	return true;
end



--�ύ����
function Task_00010506_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8930,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(10506) then
		return false;
	end

	if IsEquipTypeId(8930) then
		for k = 1, 1 do
			package:AddEquip(8930, 1);
		end
	else 
		package:AddItem(8930,1,1);
	end

	player:AddExp(600);
	player:getCoin(5300);
	return true;
end

--��������
function Task_00010506_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10506);
end
