--����Ľ�������
function Task_Accept_00050702()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 21 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50702) or task:HasCompletedTask(50702) or task:HasSubmitedTask(50702) then
		return false;
	end
	if not task:HasSubmitedTask(50701) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050702()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 21 then
		return false;
	end
	if task:HasAcceptedTask(50702) or task:HasCompletedTask(50702) or task:HasSubmitedTask(50702) then
		return false;
	end
	if not task:HasSubmitedTask(50701) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050702()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50702) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050702(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50702) == npcId and Task_Accept_00050702 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50702
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "清除怪物";
	elseif task:GetTaskSubmitNpc(50702) == npcId then
		if Task_Submit_00050702() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50702
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "清除怪物";
		elseif task:HasAcceptedTask(50702) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50702
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "清除怪物";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050702_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "哦居然真的是那群可恶的狂蛮猎人，还好我没有自己去，我非常害怕那种疯狂的猎人，而且我的茶叶还没有完全弄好，真是不好意思还得麻烦勇士你再去一次了。";
	action.m_ActionMsg = "唉还真是麻烦，只好帮你去杀掉他们了。";
	return action;
end

function Task_00050702_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "真是感谢你的帮忙啊勇士，现在我的山茶终于处理好了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050702_step_table = {
		[1] = Task_00050702_step_01,
		[10] = Task_00050702_step_10,
		};

function Task_00050702_step(step)
	if Task_00050702_step_table[step] ~= nil then
		return Task_00050702_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050702_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050702() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50702) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00050702_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(50702) then
		return false;
	end


	player:AddExp(1800);
	player:getCoin(3600);
	return true;
end

--��������
function Task_00050702_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50702);
end
