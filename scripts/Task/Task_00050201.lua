--����Ľ�������
function Task_Accept_00050201()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50201) or task:HasCompletedTask(50201) or task:HasSubmitedTask(50201) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050201()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(50201) or task:HasCompletedTask(50201) or task:HasSubmitedTask(50201) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050201()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50201) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050201(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50201) == npcId and Task_Accept_00050201 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50201
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "跑腿";
	elseif task:GetTaskSubmitNpc(50201) == npcId then
		if Task_Submit_00050201() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50201
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "跑腿";
		elseif task:HasAcceptedTask(50201) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50201
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "跑腿";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050201_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，我想你来到地界以后也听说过我们的悦意公主了，从她去了天庭我就再也没有见过她了，悦意真是一个很好的公主，为了魔族的利益牺牲了自己的幸福。";
	action.m_ActionMsg = "嗯悦意公主的故事我知道一些。";
	return action;
end

function Task_00050201_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "不知道悦意现在跟因达罗还有没有联系了，所以希望你能代我去勇健宫找因达罗问一下，说我很担心也很想念悦意，如果有什么消息希望能告诉我。";
	action.m_ActionMsg = "好的，我现在去找战神问问。";
	return action;
end

function Task_00050201_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "如果是为悦意的事情，还是不要再来找我了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050201_step_table = {
		[1] = Task_00050201_step_01,
		[2] = Task_00050201_step_02,
		[10] = Task_00050201_step_10,
		};

function Task_00050201_step(step)
	if Task_00050201_step_table[step] ~= nil then
		return Task_00050201_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050201_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050201() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50201) then
		return false;
	end
	task:AddTaskStep(50201);
	return true;
end



--�ύ����
function Task_00050201_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(50201) then
		return false;
	end


	player:AddExp(1000);
	player:getCoin(750);
	return true;
end

--��������
function Task_00050201_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50201);
end
