--����Ľ�������
function Task_Accept_00030702()
	local player = GetPlayer();
	if player:GetLev() < 47 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30702) or task:HasCompletedTask(30702) or task:HasSubmitedTask(30702) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30701) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30701) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030702()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 47 then
		return false;
	end
	if task:HasAcceptedTask(30702) or task:HasCompletedTask(30702) or task:HasSubmitedTask(30702) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30701) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30701) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030702()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30702) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030702(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30702) == npcId and Task_Accept_00030702 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30702
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "医治";
	elseif task:GetTaskSubmitNpc(30702) == npcId then
		if Task_Submit_00030702() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30702
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "医治";
		elseif task:HasAcceptedTask(30702) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30702
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "医治";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030702_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "年轻的勇士，远远的我就能感受到你体内的毒性散发出来的寒气，再不医治你会吃到更多的苦头。";
	action.m_ActionMsg = "医官，请你帮助我。";
	return action;
end

function Task_00030702_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我已经将你带来的蜘蛛毒液合成了解药，现在你需要去旁边的玄冥阵，使用解药，这样不仅能解除你体内的寒毒，而且可以使你获得抵御再次中毒的能力。";
	action.m_ActionMsg = "非常感谢您。";
	return action;
end

function Task_00030702_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "现在好了，以后你不用再为中这样的毒担心了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030702_step_table = {
		[1] = Task_00030702_step_01,
		[2] = Task_00030702_step_02,
		[10] = Task_00030702_step_10,
		};

function Task_00030702_step(step)
	if Task_00030702_step_table[step] ~= nil then
		return Task_00030702_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030702_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030702() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15047, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(30702) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00030702_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15047,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8933,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30702) then
		return false;
	end

	if IsEquipTypeId(8933) then
		for k = 1, 1 do
			package:AddEquip(8933, 1);
		end
	else 
		package:AddItem(8933,1,1);
	end
	package:DelItemAll(15047,1);

	player:AddExp(44000);
	player:getCoin(31500);
	player:getTael(10);
	return true;
end

--��������
function Task_00030702_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15047,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30702);
end
