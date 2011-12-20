--����Ľ�������
function Task_Accept_00000147()
	local player = GetPlayer();
	if player:GetLev() < 60 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(147) or task:HasCompletedTask(147) or task:HasSubmitedTask(147) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000147()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 60 then
		return false;
	end
	if task:HasAcceptedTask(147) or task:HasCompletedTask(147) or task:HasSubmitedTask(147) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000147()
	if GetPlayer():GetTaskMgr():HasCompletedTask(147) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000147(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(147) == npcId and Task_Accept_00000147 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 147
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "极恶穷奇";
	elseif task:GetTaskSubmitNpc(147) == npcId then
		if Task_Submit_00000147() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 147
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "极恶穷奇";
		elseif task:HasAcceptedTask(147) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 147
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "极恶穷奇";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000147_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这穷奇原是四凶之一，与无华氏父子争斗，由此相识狼狈为奸，又破了昔年白阳真人禁制三尸墓的禁法，从此更是嚣张，不过这穷奇乃是四凶之一，颇有些本领，少侠你须得小心应付才是。";
	action.m_ActionMsg = "管他什么妖魔鬼怪，我只凭手中飞剑，自叫他有来无回。";
	return action;
end

function Task_00000147_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."，少侠真是剑术高强，一身正气啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000147_step_table = {
		[1] = Task_00000147_step_01,
		[10] = Task_00000147_step_10,
		};

function Task_00000147_step(step)
	if Task_00000147_step_table[step] ~= nil then
		return Task_00000147_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000147_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000147() then
		return false;
	end
	if not task:AcceptTask(147) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000147_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(147) then
		return false;
	end


	player:AddExp(90000);
	return true;
end

--��������
function Task_00000147_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(147);
end
