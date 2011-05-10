--����Ľ�������
function Task_Accept_00031201()
	local player = GetPlayer();
	if player:GetLev() < 59 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31201) or task:HasCompletedTask(31201) or task:HasSubmitedTask(31201) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31104) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31004) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031201()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 59 then
		return false;
	end
	if task:HasAcceptedTask(31201) or task:HasCompletedTask(31201) or task:HasSubmitedTask(31201) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31104) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31004) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031201()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31201) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031201(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31201) == npcId and Task_Accept_00031201 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31201
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "摩睺的利用";
	elseif task:GetTaskSubmitNpc(31201) == npcId then
		if Task_Submit_00031201() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31201
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "摩睺的利用";
		elseif task:HasAcceptedTask(31201) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31201
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "摩睺的利用";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031201_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "从这里路过的这位勇士，可否愿意为我做点事情呢？";
	action.m_ActionMsg = "你是谁？我为什么要为你做事情？";
	return action;
end

function Task_00031201_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我就是一直住在这里的居民啊？其实只要你愿意为我做一点事情，就会获得丰厚的回报，我可是知道，你奔波一路，总是想要寻找名将，这些都是我可以回报给你的。如果你有足够的实力，那就去帮我训服可林谷的那些疯狂雪怪吧。";
	action.m_ActionMsg = "那有什么难，希望回来以后你会兑现你的承诺。";
	return action;
end

function Task_00031201_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "年轻的勇士做的不错嘛，这么快就回来了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031201_step_table = {
		[1] = Task_00031201_step_01,
		[2] = Task_00031201_step_02,
		[10] = Task_00031201_step_10,
		};

function Task_00031201_step(step)
	if Task_00031201_step_table[step] ~= nil then
		return Task_00031201_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031201_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031201() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31201) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00031201_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(31201) then
		return false;
	end


	player:AddExp(100000);
	player:getCoin(100000);
	player:getTael(30);
	return true;
end

--��������
function Task_00031201_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31201);
end
