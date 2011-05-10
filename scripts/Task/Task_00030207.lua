--����Ľ�������
function Task_Accept_00030207()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 32 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30207) or task:HasCompletedTask(30207) or task:HasSubmitedTask(30207) then
		return false;
	end
	if not task:HasSubmitedTask(30206) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030207()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 32 then
		return false;
	end
	if task:HasAcceptedTask(30207) or task:HasCompletedTask(30207) or task:HasSubmitedTask(30207) then
		return false;
	end
	if not task:HasSubmitedTask(30206) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00030207()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30207) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030207(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30207) == npcId and Task_Accept_00030207 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30207
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "冒险旅程";
	elseif task:GetTaskSubmitNpc(30207) == npcId then
		if Task_Submit_00030207() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30207
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "冒险旅程";
		elseif task:HasAcceptedTask(30207) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30207
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "冒险旅程";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030207_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "非常感谢您对我们做的所有帮助，茫茫山海大陆，您的到来对我们就是雪中送炭，现在开始我们又能以更充足的士气驻守边防，保护我们魔族的势力了。";
	action.m_ActionMsg = "太客气了，魔族也要感谢你们的付出。";
	return action;
end

function Task_00030207_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士当您离开这里以后，山海大陆会有各种险恶的挑战等着您，提升自己的实力一定是最重要的，我们在这里也帮不您更多，带着这封介绍信去阿兰若山谷的万人墓，找那里的守墓人吧，他是一个古怪的老头，没有人介绍他不会理您的，可他却是一个高深莫测的人，会给您在山海大陆的旅程有所帮助的。";
	action.m_ActionMsg = "你太客气了，为魔族效力是我分内之事.";
	return action;
end

function Task_00030207_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这位风尘仆仆的勇士，你到这里来有何贵干？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030207_step_table = {
		[1] = Task_00030207_step_01,
		[2] = Task_00030207_step_02,
		[10] = Task_00030207_step_10,
		};

function Task_00030207_step(step)
	if Task_00030207_step_table[step] ~= nil then
		return Task_00030207_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030207_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030207() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15033, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(30207) then
		return false;
	end
	task:AddTaskStep(30207);
	return true;
end



--�ύ����
function Task_00030207_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15033,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(306,1,1) + package:GetItemUsedGrids(8914,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30207) then
		return false;
	end

	if IsEquipTypeId(306) then
		for k = 1, 1 do
			package:AddEquip(306, 1);
		end
	else 
		package:AddItem(306,1,1);
	end
	if IsEquipTypeId(8914) then
		for k = 1, 1 do
			package:AddEquip(8914, 1);
		end
	else 
		package:AddItem(8914,1,1);
	end
	package:DelItemAll(15033,1);

	player:AddExp(4500);
	player:getCoin(5900);
	player:getTael(20);
	return true;
end

--��������
function Task_00030207_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15033,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30207);
end
