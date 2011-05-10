--����Ľ�������
function Task_Accept_00030202()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30202) or task:HasCompletedTask(30202) or task:HasSubmitedTask(30202) then
		return false;
	end
	if not task:HasSubmitedTask(30201) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030202()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(30202) or task:HasCompletedTask(30202) or task:HasSubmitedTask(30202) then
		return false;
	end
	if not task:HasSubmitedTask(30201) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00030202()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30202) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030202(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30202) == npcId and Task_Accept_00030202 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30202
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "天族复仇者";
	elseif task:GetTaskSubmitNpc(30202) == npcId then
		if Task_Submit_00030202() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30202
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "天族复仇者";
		elseif task:HasAcceptedTask(30202) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30202
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "天族复仇者";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030202_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "现在我们魔族的棋子被立好以后，一定会有很多疯狂的天族复仇者聚集在观旗台外面的空地上的，我希望你去帮我查看一下，不过在了解他们的实力之前，不要贸然动手。";
	action.m_ActionMsg = "好的，我去看看。";
	return action;
end

function Task_00030202_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那里的情况怎么样？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030202_step_table = {
		[1] = Task_00030202_step_01,
		[10] = Task_00030202_step_10,
		};

function Task_00030202_step(step)
	if Task_00030202_step_table[step] ~= nil then
		return Task_00030202_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030202_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030202() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30202) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00030202_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30202) then
		return false;
	end


	player:AddExp(4000);
	player:getCoin(5500);
	player:getTael(10);
	return true;
end

--��������
function Task_00030202_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30202);
end
