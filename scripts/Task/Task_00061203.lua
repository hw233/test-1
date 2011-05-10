--����Ľ�������
function Task_Accept_00061203()
	local player = GetPlayer();
	if player:GetLev() < 51 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61203) or task:HasCompletedTask(61203) or task:HasSubmitedTask(61203) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61202) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61202) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061203()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 51 then
		return false;
	end
	if task:HasAcceptedTask(61203) or task:HasCompletedTask(61203) or task:HasSubmitedTask(61203) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61202) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61202) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061203()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61203) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061203(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61203) == npcId and Task_Accept_00061203 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61203
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "安抚路之遥";
	elseif task:GetTaskSubmitNpc(61203) == npcId then
		if Task_Submit_00061203() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61203
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "安抚路之遥";
		elseif task:HasAcceptedTask(61203) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61203
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "安抚路之遥";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061203_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "现在我觉得整个天池已经好多了，没有那些烦人的怪物来破坏风景，我可以好好在这里享受。";
	action.m_ActionMsg = "你还真挺会选地方的呢。";
	return action;
end

function Task_00061203_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "对了，请你去回复一下路之遥，说很感谢他送来点心，天池已经被整治的和谐了许多，让他不必担心，安心做好厨子，他手艺这么好，以后少不了要麻烦他的。";
	action.m_ActionMsg = "行，那我去告诉他，顺便也尝尝他的手艺，哈哈。";
	return action;
end

function Task_00061203_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "一把冷汗啊，这公主果然很不省油。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061203_step_table = {
		[1] = Task_00061203_step_01,
		[2] = Task_00061203_step_02,
		[10] = Task_00061203_step_10,
		};

function Task_00061203_step(step)
	if Task_00061203_step_table[step] ~= nil then
		return Task_00061203_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061203_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061203() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61203) then
		return false;
	end
	task:AddTaskStep(61203);
	return true;
end



--�ύ����
function Task_00061203_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8906,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(61203) then
		return false;
	end

	if IsEquipTypeId(8906) then
		for k = 1, 1 do
			package:AddEquip(8906, 1);
		end
	else 
		package:AddItem(8906,1,1);
	end

	player:AddExp(60000);
	player:getCoin(47000);
	return true;
end

--��������
function Task_00061203_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61203);
end
