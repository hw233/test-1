--����Ľ�������
function Task_Accept_00010401()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 23 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10401) or task:HasCompletedTask(10401) or task:HasSubmitedTask(10401) then
		return false;
	end
	if not task:HasSubmitedTask(10309) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010401()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 23 then
		return false;
	end
	if task:HasAcceptedTask(10401) or task:HasCompletedTask(10401) or task:HasSubmitedTask(10401) then
		return false;
	end
	if not task:HasSubmitedTask(10309) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010401()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10401) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010401(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10401) == npcId and Task_Accept_00010401 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10401
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "难兄的坟墓";
	elseif task:GetTaskSubmitNpc(10401) == npcId then
		if Task_Submit_00010401() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10401
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "难兄的坟墓";
		elseif task:HasAcceptedTask(10401) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10401
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "难兄的坟墓";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010401_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "既然婆修不能亲自来，那你就和我一起去吧，今天我们该去给我和婆修最好的兄弟神秀上香了，我们三个从小一起习武修行，曾经发誓同生共死，可是由于惨烈的天魔之战，在一次危险中神秀为了保护我和婆修而失去了生命。";
	action.m_ActionMsg = "失去了最好的兄弟的确很让人难过，请节哀。";
	return action;
end

function Task_00010401_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "之后的每年，我都会和婆修在这个时候来给神秀上香，即使婆修忙到不能亲自来，也会派个人来代替他。现在你去坟上把手里的那束香火点上吧。";
	action.m_ActionMsg = "婆修也吩咐过我了，我这就去把香火点上。";
	return action;
end

function Task_00010401_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "希望跟我们已经不在一个世界的神秀，能够感受到我们对他的怀念。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010401_step_table = {
		[1] = Task_00010401_step_01,
		[2] = Task_00010401_step_02,
		[10] = Task_00010401_step_10,
		};

function Task_00010401_step(step)
	if Task_00010401_step_table[step] ~= nil then
		return Task_00010401_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010401_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010401() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15078, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(10401) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00010401_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15078,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(10401) then
		return false;
	end

	package:DelItemAll(15078,1);

	player:AddExp(2200);
	player:getCoin(3500);
	player:getTael(10);
	return true;
end

--��������
function Task_00010401_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15078,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10401);
end
