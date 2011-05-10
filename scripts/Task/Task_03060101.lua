--����Ľ�������
function Task_Accept_03060101()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 44 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(3060101) or task:HasCompletedTask(3060101) or task:HasSubmitedTask(3060101) then
		return false;
	end
	if not task:HasSubmitedTask(30507) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_03060101()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 44 then
		return false;
	end
	if task:HasAcceptedTask(3060101) or task:HasCompletedTask(3060101) or task:HasSubmitedTask(3060101) then
		return false;
	end
	if not task:HasSubmitedTask(30507) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_03060101()
	if GetPlayer():GetTaskMgr():HasCompletedTask(3060101) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_03060101(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(3060101) == npcId and Task_Accept_03060101 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 3060101
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "路遇小鬼";
	elseif task:GetTaskSubmitNpc(3060101) == npcId then
		if Task_Submit_03060101() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 3060101
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "路遇小鬼";
		elseif task:HasAcceptedTask(3060101) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 3060101
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "路遇小鬼";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_03060101_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "喂，那边那位勇士，能过来一下吗？我现在急需帮助，我的兄弟受伤了，他在对付你的敌人魔族的战斗中受了伤，你能帮我吗？";
	action.m_ActionMsg = "原来是帮我们对付魔族的兄弟啊，我该怎么帮你呢？";
	return action;
end

function Task_03060101_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那些魔族的敌人太可恨了，把我的兄弟伤成这样，我想你一定不会见死不救，这么重的伤只有你们族内的特产甘露可以救活他，只需要一点点，就能换回一条生命，你愿意帮助我们吗？\n";
	action.m_ActionMsg = "我可以试试。";
	return action;
end

function Task_03060101_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是我们天族的勇士"..GetPlayerName(GetPlayer()).."回来了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_03060101_step_table = {
		[1] = Task_03060101_step_01,
		[2] = Task_03060101_step_02,
		[10] = Task_03060101_step_10,
		};

function Task_03060101_step(step)
	if Task_03060101_step_table[step] ~= nil then
		return Task_03060101_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_03060101_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_03060101() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(3060101) then
		return false;
	end
	task:AddTaskStep(3060101);
	return true;
end



--�ύ����
function Task_03060101_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(3060101) then
		return false;
	end


	player:AddExp(32000);
	player:getCoin(22000);
	return true;
end

--��������
function Task_03060101_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(3060101);
end
