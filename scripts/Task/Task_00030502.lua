--����Ľ�������
function Task_Accept_00030502()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30502) or task:HasCompletedTask(30502) or task:HasSubmitedTask(30502) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30501) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30501) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030502()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(30502) or task:HasCompletedTask(30502) or task:HasSubmitedTask(30502) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30501) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30501) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030502()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30502) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030502(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30502) == npcId and Task_Accept_00030502 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30502
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "再会帮主";
	elseif task:GetTaskSubmitNpc(30502) == npcId then
		if Task_Submit_00030502() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30502
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "再会帮主";
		elseif task:HasAcceptedTask(30502) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30502
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "再会帮主";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030502_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士"..GetPlayerName(GetPlayer()).."，其实不必为我们的事情担心，人生苦短，有所追求是幸福的，能不能得到也是看运气，就算如影一直不能接受我，我也愿意像现在这样默默的支持他。";
	action.m_ActionMsg = "我很为你的精神感动。";
	return action;
end

function Task_00030502_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "如影现在只是一心的想要打击夜摩盟的势力，他心理只有这个执着的信念，我在这里开旅店，离他不远，却也不近，勇士，如果你时间上允许，请帮我将这些银票带去东山城送给如影，希望能帮到他。";
	action.m_ActionMsg = "没问题，我去送给他。";
	return action;
end

function Task_00030502_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你离开的这段时间，都去了些什么地方啊？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030502_step_table = {
		[1] = Task_00030502_step_01,
		[2] = Task_00030502_step_02,
		[10] = Task_00030502_step_10,
		};

function Task_00030502_step(step)
	if Task_00030502_step_table[step] ~= nil then
		return Task_00030502_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030502_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030502() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15041, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(30502) then
		return false;
	end
	task:AddTaskStep(30502);
	return true;
end



--�ύ����
function Task_00030502_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15041,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8922,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30502) then
		return false;
	end

	if IsEquipTypeId(8922) then
		for k = 1, 1 do
			package:AddEquip(8922, 1);
		end
	else 
		package:AddItem(8922,1,1);
	end
	package:DelItemAll(15041,1);

	player:AddExp(10000);
	player:getCoin(13500);
	player:getTael(10);
	return true;
end

--��������
function Task_00030502_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15041,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30502);
end
