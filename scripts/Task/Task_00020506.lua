--����Ľ�������
function Task_Accept_00020506()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 25 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20506) or task:HasCompletedTask(20506) or task:HasSubmitedTask(20506) then
		return false;
	end
	if not task:HasSubmitedTask(20505) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020506()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 25 then
		return false;
	end
	if task:HasAcceptedTask(20506) or task:HasCompletedTask(20506) or task:HasSubmitedTask(20506) then
		return false;
	end
	if not task:HasSubmitedTask(20505) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020506()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20506) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020506(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20506) == npcId and Task_Accept_00020506 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20506
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "前往山海大陆";
	elseif task:GetTaskSubmitNpc(20506) == npcId then
		if Task_Submit_00020506() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20506
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "前往山海大陆";
		elseif task:HasAcceptedTask(20506) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20506
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
function Task_00020506_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "夜叉王和摩可拿还真是狡猾，明明发现他们的踪迹了，却又逃跑了。不过能够消灭侵袭到这儿的狼鬼族势力，我认为你已经做的很好了。";
	action.m_ActionMsg = "感谢对我的肯定。";
	return action;
end

function Task_00020506_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我很欣赏你的精神，我想你已经见识到我们正面对的仇恨和威胁，你的实力需要增强，你的战斗队伍需要壮大，从这儿的出口出去，到那片充满挑战的山海大陆去吧，迎接新的挑战，寻找更多的名将，位于阿兰若古城的魔族前线营地的士兵会接待你的。";
	action.m_ActionMsg = "我愿意去山海大陆接受挑战。";
	return action;
end

function Task_00020506_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "欢迎你，初来山海大陆的魔族勇士。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020506_step_table = {
		[1] = Task_00020506_step_01,
		[2] = Task_00020506_step_02,
		[10] = Task_00020506_step_10,
		};

function Task_00020506_step(step)
	if Task_00020506_step_table[step] ~= nil then
		return Task_00020506_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020506_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020506() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20506) then
		return false;
	end
	task:AddTaskStep(20506);
	return true;
end



--�ύ����
function Task_00020506_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8930,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(20506) then
		return false;
	end

	if IsEquipTypeId(8930) then
		for k = 1, 1 do
			package:AddEquip(8930, 1);
		end
	else 
		package:AddItem(8930,1,1);
	end

	player:AddExp(4000);
	player:getCoin(5300);
	return true;
end

--��������
function Task_00020506_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20506);
end
