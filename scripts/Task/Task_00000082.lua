--����Ľ�������
function Task_Accept_00000082()
	local player = GetPlayer();
	if player:GetLev() < 45 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(82) or task:HasCompletedTask(82) or task:HasSubmitedTask(82) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000082()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 45 then
		return false;
	end
	if task:HasAcceptedTask(82) or task:HasCompletedTask(82) or task:HasSubmitedTask(82) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000082()
	if GetPlayer():GetTaskMgr():HasCompletedTask(82) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000082(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(82) == npcId and Task_Accept_00000082 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 82
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "长沙送信";
	elseif task:GetTaskSubmitNpc(82) == npcId then
		if Task_Submit_00000082() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 82
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "长沙送信";
		elseif task:HasAcceptedTask(82) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 82
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "长沙送信";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000082_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "戴家场这边的风波也算顺利解决了，不过我这次本来是要去青螺山和其他剑仙会和的。那里有一处青螺魔宫，八个魔头聚在那里为祸，现在时间紧迫，我不能再做耽搁了。";
	action.m_ActionMsg = "前辈那就赶快上路吧。";
	return action;
end

function Task_00000082_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我这里有书信一封，原本是要交给长沙白龙庵的素因大师，邀她同去青螺山的，现在只有麻烦你帮我送去了。";
	action.m_ActionMsg = "没问题，就交给我吧。";
	return action;
end

function Task_00000082_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "多谢少侠千里送信来此。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000082_step_table = {
		[1] = Task_00000082_step_01,
		[2] = Task_00000082_step_02,
		[10] = Task_00000082_step_10,
		};

function Task_00000082_step(step)
	if Task_00000082_step_table[step] ~= nil then
		return Task_00000082_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000082_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000082() then
		return false;
	end
	if not task:AcceptTask(82) then
		return false;
	end
	task:AddTaskStep(82);
	return true;
end



--�ύ����
function Task_00000082_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(82) then
		return false;
	end


	player:AddExp(25000);
	return true;
end

--��������
function Task_00000082_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(82);
end
