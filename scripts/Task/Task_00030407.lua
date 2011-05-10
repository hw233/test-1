--����Ľ�������
function Task_Accept_00030407()
	local player = GetPlayer();
	if player:GetLev() < 37 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30407) or task:HasCompletedTask(30407) or task:HasSubmitedTask(30407) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30406) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30406) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030407()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 37 then
		return false;
	end
	if task:HasAcceptedTask(30407) or task:HasCompletedTask(30407) or task:HasSubmitedTask(30407) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30406) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30406) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030407()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30407) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030407(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30407) == npcId and Task_Accept_00030407 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30407
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "锻造武器";
	elseif task:GetTaskSubmitNpc(30407) == npcId then
		if Task_Submit_00030407() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30407
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "锻造武器";
		elseif task:HasAcceptedTask(30407) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30407
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "锻造武器";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030407_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "山河城欢迎你，"..GetStateName(GetPlayer():getCountry()).."的勇士"..GetPlayerName(GetPlayer()).."，从你装备上的印记我就知道，你已经见过东山派的帮主如影了，如影是我最好的兄弟，我一向非常支持他所有的行动。";
	action.m_ActionMsg = "能认识您我很荣幸。";
	return action;
end

function Task_00030407_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "为了支持如影，我决定帮你打造一把锐利的武器，这把武器需要高级的铸器师和上等的材料，现在你去南面回音山的碧玉矿坑采一块碧玉石回来，有一个著名的铸器师，来无影去无踪，只有在黑森林的茅屋才找的到他。";
	action.m_ActionMsg = "我这就去了。";
	return action;
end

function Task_00030407_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我这偏僻的茅屋，难得有生人来啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030407_step_table = {
		[1] = Task_00030407_step_01,
		[2] = Task_00030407_step_02,
		[10] = Task_00030407_step_10,
		};

function Task_00030407_step(step)
	if Task_00030407_step_table[step] ~= nil then
		return Task_00030407_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030407_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030407() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30407) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15038,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15038, itemNum, 1);
		end
		task:AddTaskStep2(30407, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00030407_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15038,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8914,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30407) then
		return false;
	end

	if IsEquipTypeId(8914) then
		for k = 1, 1 do
			package:AddEquip(8914, 1);
		end
	else 
		package:AddItem(8914,1,1);
	end
	package:DelItemAll(15038,1);

	player:AddExp(6000);
	player:getCoin(9500);
	player:getTael(10);
	return true;
end

--��������
function Task_00030407_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15038,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30407);
end
