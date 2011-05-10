--����Ľ�������
function Task_Accept_00050301()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 10 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50301) or task:HasCompletedTask(50301) or task:HasSubmitedTask(50301) then
		return false;
	end
	if not task:HasSubmitedTask(50105) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050301()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 10 then
		return false;
	end
	if task:HasAcceptedTask(50301) or task:HasCompletedTask(50301) or task:HasSubmitedTask(50301) then
		return false;
	end
	if not task:HasSubmitedTask(50105) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050301()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50301) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050301(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50301) == npcId and Task_Accept_00050301 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50301
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "莫名挑衅";
	elseif task:GetTaskSubmitNpc(50301) == npcId then
		if Task_Submit_00050301() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50301
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "莫名挑衅";
		elseif task:HasAcceptedTask(50301) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50301
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "莫名挑衅";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050301_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，看你这意气风发的样子，是不是对自己特别自信啊，可是你知不知道，有人其实根本就瞧不起你。";
	action.m_ActionMsg = "你说什么？谁敢瞧不起我？";
	return action;
end

function Task_00050301_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "当然是灰烬谷的韩世忠了，这家伙跟我一起来到你们地界，可是他不屑于跟我一起忠于阿修罗，一个人跑到灰烬谷躲了起来。";
	action.m_ActionMsg = "别说了，我马上就找他对峙。";
	return action;
end

function Task_00050301_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，干吗这么气冲冲的。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050301_step_table = {
		[1] = Task_00050301_step_01,
		[2] = Task_00050301_step_02,
		[10] = Task_00050301_step_10,
		};

function Task_00050301_step(step)
	if Task_00050301_step_table[step] ~= nil then
		return Task_00050301_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050301_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050301() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50301) then
		return false;
	end
	task:AddTaskStep(50301);
	return true;
end



--�ύ����
function Task_00050301_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(50301) then
		return false;
	end


	player:AddExp(1000);
	player:getCoin(850);
	return true;
end

--��������
function Task_00050301_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50301);
end
