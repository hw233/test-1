--����Ľ�������
function Task_Accept_00031603()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 67 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31603) or task:HasCompletedTask(31603) or task:HasSubmitedTask(31603) then
		return false;
	end
	if not task:HasSubmitedTask(31602) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031603()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 67 then
		return false;
	end
	if task:HasAcceptedTask(31603) or task:HasCompletedTask(31603) or task:HasSubmitedTask(31603) then
		return false;
	end
	if not task:HasSubmitedTask(31602) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00031603()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31603) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031603(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31603) == npcId and Task_Accept_00031603 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31603
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "实力证明";
	elseif task:GetTaskSubmitNpc(31603) == npcId then
		if Task_Submit_00031603() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31603
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "实力证明";
		elseif task:HasAcceptedTask(31603) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31603
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "实力证明";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031603_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "尽管你是这样说的，可是我仍然很难相信会是这样，就算吕布是因为投靠了夜摩盟而感到后悔，可是无论如何他也是少有的世间名将，有着强大的实力，不会这么轻易的就被你打败的。";
	action.m_ActionMsg = "虽然我并不是故意要杀死吕布，但是我的实力也已经有了很大的增强。";
	return action;
end

function Task_00031603_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "既然你这么说，如果你有胆量去跟番禹谷跟我们的魔族前锋军挑战并且胜利，我就相信你的话。";
	action.m_ActionMsg = "我一定会证明给你看的。";
	return action;
end

function Task_00031603_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "居然真的做到了，难道我真的要相信你这来自天族的人。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031603_step_table = {
		[1] = Task_00031603_step_01,
		[2] = Task_00031603_step_02,
		[10] = Task_00031603_step_10,
		};

function Task_00031603_step(step)
	if Task_00031603_step_table[step] ~= nil then
		return Task_00031603_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031603_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031603() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31603) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00031603_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(847,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31603) then
		return false;
	end

	if IsEquipTypeId(847) then
		for k = 1, 1 do
			package:AddEquip(847, 1);
		end
	else 
		package:AddItem(847,1,1);
	end

	player:AddExp(200000);
	player:getCoin(180000);
	player:getTael(50);
	return true;
end

--��������
function Task_00031603_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31603);
end
