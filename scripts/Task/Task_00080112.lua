--����Ľ�������
function Task_Accept_00080112()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80112) or task:HasCompletedTask(80112) or (task:HasSubmitedTask(80112) and (GetSharpDay(task:GetTaskEndTime(80112)) == GetSharpDay(os.time()))) then 
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080112()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(80112) or task:HasCompletedTask(80112) or (task:HasSubmitedTask(80112) and (GetSharpDay(task:GetTaskEndTime(80112)) == GetSharpDay(os.time()))) then 
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00080112()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80112) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080112(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80112) == npcId and Task_Accept_00080112 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80112
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "名将嗜好";
	elseif task:GetTaskSubmitNpc(80112) == npcId then
		if Task_Submit_00080112() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80112
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "名将嗜好";
		elseif task:HasAcceptedTask(80112) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80112
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "名将嗜好";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080112_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "江湖险恶，你需要去结交一些名将，他们各自有着不同的能力能帮助你作战。但首先需要你花些功夫去找他们喜欢的东西，然后送给他们以表诚意。";
	action.m_ActionMsg = "千里难寻是朋友，我可以给不同的名将送东西吗？";
	return action;
end

function Task_00080112_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "当然可以，朋友多了路好走嘛。";
	action.m_ActionMsg = "那我得赶紧去寻找他们喜欢的东西。";
	return action;
end

function Task_00080112_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你结交到朋友了吗？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080112_step_table = {
		[1] = Task_00080112_step_01,
		[2] = Task_00080112_step_02,
		[10] = Task_00080112_step_10,
		};

function Task_00080112_step(step)
	if Task_00080112_step_table[step] ~= nil then
		return Task_00080112_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080112_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080112() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80112) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00080112_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80112) then
		return false;
	end


	player:AddExp(20000);
	return true;
end

--��������
function Task_00080112_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80112);
end
