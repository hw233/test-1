--����Ľ�������
function Task_Accept_00080075()
	local player = GetPlayer();
	if player:GetLev() < 75 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80075) or task:HasCompletedTask(80075) or task:HasSubmitedTask(80075) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080075()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 75 then
		return false;
	end
	if task:HasAcceptedTask(80075) or task:HasCompletedTask(80075) or task:HasSubmitedTask(80075) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00080075()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80075) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080075(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80075) == npcId and Task_Accept_00080075 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80075
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "分外眼红";
	elseif task:GetTaskSubmitNpc(80075) == npcId then
		if Task_Submit_00080075() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80075
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "分外眼红";
		elseif task:HasAcceptedTask(80075) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80075
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "分外眼红";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080075_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，修炼的历程是艰辛而曲折的，山海大陆的想必你已经有所了解。我急需你的帮助，去杀掉那些在环海城灭境岩的裂化渔人，收集7000个他们的灰暗鱼鳞，我现在不得不借助你的力量，来平息这样的状况，而且这同时能给你的修炼带来收获。";
	action.m_ActionMsg = "是个不错的主意，我愿意接受。";
	return action;
end

function Task_00080075_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "的确是一举两得的事情。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080075_step_table = {
		[1] = Task_00080075_step_01,
		[10] = Task_00080075_step_10,
		};

function Task_00080075_step(step)
	if Task_00080075_step_table[step] ~= nil then
		return Task_00080075_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080075_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080075() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80075) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15099,1);
	if itemNum ~= 0 then
		if itemNum > 80 then
			itemNum = 80;
			package:SetItem(15099, itemNum, 1);
		end
		task:AddTaskStep2(80075, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00080075_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15099,1) < 80 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(80075) then
		return false;
	end

	package:DelItemAll(15099,1);

	player:AddExp(440000);
	player:getTael(200);
	return true;
end

--��������
function Task_00080075_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15099,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80075);
end
