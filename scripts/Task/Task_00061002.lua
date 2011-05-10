--����Ľ�������
function Task_Accept_00061002()
	local player = GetPlayer();
	if player:GetLev() < 48 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61002) or task:HasCompletedTask(61002) or task:HasSubmitedTask(61002) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61001) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61001) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061002()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 48 then
		return false;
	end
	if task:HasAcceptedTask(61002) or task:HasCompletedTask(61002) or task:HasSubmitedTask(61002) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61001) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61001) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061002()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61002) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061002(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61002) == npcId and Task_Accept_00061002 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61002
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "学会放手";
	elseif task:GetTaskSubmitNpc(61002) == npcId then
		if Task_Submit_00061002() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61002
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "学会放手";
		elseif task:HasAcceptedTask(61002) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61002
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "学会放手";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061002_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，你找的大师难道没有治好紫衣姑娘的失语症吗？";
	action.m_ActionMsg = "紫衣姑娘的病是治好了，但她爱的人是盛一刀啊，不想离开他。";
	return action;
end

function Task_00061002_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "看到你没有和紫衣一起回来，我就已经想到了这样的结果了，唉，都怪我年轻的时候太过关注自己的事业，忽视了紫衣的感情，虽然我已经功成名就，逝去的感情却找不回来了。只能这样了，请你回复盛一刀吧，希望以后能好好照顾紫衣。";
	action.m_ActionMsg = "放手也是一种选择，希望城主你能想开。";
	return action;
end

function Task_00061002_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，你和天武就放心吧，这么多年我从来没有对紫衣不好过，我肯定会让她过的更好。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061002_step_table = {
		[1] = Task_00061002_step_01,
		[2] = Task_00061002_step_02,
		[10] = Task_00061002_step_10,
		};

function Task_00061002_step(step)
	if Task_00061002_step_table[step] ~= nil then
		return Task_00061002_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061002_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061002() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61002) then
		return false;
	end
	task:AddTaskStep(61002);
	return true;
end



--�ύ����
function Task_00061002_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8912,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(61002) then
		return false;
	end

	if IsEquipTypeId(8912) then
		for k = 1, 1 do
			package:AddEquip(8912, 1);
		end
	else 
		package:AddItem(8912,1,1);
	end

	player:AddExp(48000);
	player:getCoin(34000);
	return true;
end

--��������
function Task_00061002_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61002);
end
