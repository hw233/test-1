--����Ľ�������
function Task_Accept_00080072()
	local player = GetPlayer();
	if player:GetLev() < 72 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80072) or task:HasCompletedTask(80072) or task:HasSubmitedTask(80072) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080072()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 72 then
		return false;
	end
	if task:HasAcceptedTask(80072) or task:HasCompletedTask(80072) or task:HasSubmitedTask(80072) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00080072()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80072) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080072(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80072) == npcId and Task_Accept_00080072 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80072
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "斩妖除魔";
	elseif task:GetTaskSubmitNpc(80072) == npcId then
		if Task_Submit_00080072() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80072
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "斩妖除魔";
		elseif task:HasAcceptedTask(80072) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80072
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "斩妖除魔";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080072_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，修炼的历程是艰辛而曲折的，山海大陆的想必你已经有所了解。我急需你的帮助，去杀掉那些在环魔金矿的环魔矿工，收集70个他们的聚晶矿石，我现在不得不借助你的力量，来平息这样的状况，而且这同时能给你的修炼带来收获。";
	action.m_ActionMsg = "是个不错的主意，我愿意接受。";
	return action;
end

function Task_00080072_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这么困难的任务，居然就被你做完了。恭喜你进入更高级的修炼阶段。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080072_step_table = {
		[1] = Task_00080072_step_01,
		[10] = Task_00080072_step_10,
		};

function Task_00080072_step(step)
	if Task_00080072_step_table[step] ~= nil then
		return Task_00080072_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080072_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080072() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80072) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15098,1);
	if itemNum ~= 0 then
		if itemNum > 70 then
			itemNum = 70;
			package:SetItem(15098, itemNum, 1);
		end
		task:AddTaskStep2(80072, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00080072_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15098,1) < 70 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8934,2,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(80072) then
		return false;
	end

	if IsEquipTypeId(8934) then
		for k = 1, 2 do
			package:AddEquip(8934, 1);
		end
	else 
		package:AddItem(8934,2,1);
	end
	package:DelItemAll(15098,1);

	player:AddExp(400000);
	return true;
end

--��������
function Task_00080072_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15098,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80072);
end
