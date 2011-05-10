--����Ľ�������
function Task_Accept_00020205()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20205) or task:HasCompletedTask(20205) or task:HasSubmitedTask(20205) then
		return false;
	end
	if not task:HasSubmitedTask(20204) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020205()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(20205) or task:HasCompletedTask(20205) or task:HasSubmitedTask(20205) then
		return false;
	end
	if not task:HasSubmitedTask(20204) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020205()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20205) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020205(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20205) == npcId and Task_Accept_00020205 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20205
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "回复密使";
	elseif task:GetTaskSubmitNpc(20205) == npcId then
		if Task_Submit_00020205() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20205
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "回复密使";
		elseif task:HasAcceptedTask(20205) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20205
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "回复密使";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020205_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "毗摩质多罗向你讲清楚我们曾经的经历了吧，虽然我们和天族之间已经达成讲和的协议了，可是帝释天先待悦意不善，我想他以后还会做出更出格的事情来，世界不会一直这么太平的，你要记住我们的仇恨和责任，使自己更加强大，魔族以后需要你。";
	action.m_ActionMsg = "帝释天实在是太过火了。";
	return action;
end

function Task_00020205_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我想尽快跟公主取得联系，你希望你在5分钟之内把这封信送出去给在城外的魔族密使，让他尽快把这封信送给悦意公主。";
	action.m_ActionMsg = "我会马上去。";
	return action;
end

function Task_00020205_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你怎么这么急匆匆的赶来？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020205_step_table = {
		[1] = Task_00020205_step_01,
		[2] = Task_00020205_step_02,
		[10] = Task_00020205_step_10,
		};

function Task_00020205_step(step)
	if Task_00020205_step_table[step] ~= nil then
		return Task_00020205_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020205_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020205() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15028, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(20205) then
		return false;
	end
	task:AddTaskStep(20205);
	return true;
end



--�ύ����
function Task_00020205_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15028,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(20205) then
		return false;
	end

	package:DelItemAll(15028,1);

	player:AddExp(1400);
	player:getCoin(1300);
	return true;
end

--��������
function Task_00020205_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15028,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20205);
end
