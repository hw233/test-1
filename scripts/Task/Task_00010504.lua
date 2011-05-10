--����Ľ�������
function Task_Accept_00010504()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 26 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10504) or task:HasCompletedTask(10504) or task:HasSubmitedTask(10504) then
		return false;
	end
	if not task:HasSubmitedTask(10503) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010504()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 26 then
		return false;
	end
	if task:HasAcceptedTask(10504) or task:HasCompletedTask(10504) or task:HasSubmitedTask(10504) then
		return false;
	end
	if not task:HasSubmitedTask(10503) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010504()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10504) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010504(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10504) == npcId and Task_Accept_00010504 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10504
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "力量的测试";
	elseif task:GetTaskSubmitNpc(10504) == npcId then
		if Task_Submit_00010504() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10504
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "力量的测试";
		elseif task:HasAcceptedTask(10504) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10504
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "力量的测试";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010504_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "通过刚刚的战斗，我想勇士你应该已经意识到自己的实力了。";
	action.m_ActionMsg = "原来我还没有成长到很强大，我很遗憾。";
	return action;
end

function Task_00010504_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "虽然你努力训练和成长，但是毕竟还不够强壮，不能贸然派你去面对危险，那群夜摩盟里，很可能就有夜叉王和摩可拿在里面，他们的能力可是超出你非常多。这样吧，你去金山道场找大德法师，他是一个法术非常高超的法师，他一定会帮助你的。";
	action.m_ActionMsg = "其实我还是很厉害的，不过我还是需要法师的帮助吧。";
	return action;
end

function Task_00010504_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我很遗憾。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010504_step_table = {
		[1] = Task_00010504_step_01,
		[2] = Task_00010504_step_02,
		[10] = Task_00010504_step_10,
		};

function Task_00010504_step(step)
	if Task_00010504_step_table[step] ~= nil then
		return Task_00010504_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010504_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010504() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(10504) then
		return false;
	end
	task:AddTaskStep(10504);
	return true;
end



--�ύ����
function Task_00010504_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(10504) then
		return false;
	end


	player:AddExp(3500);
	player:getCoin(5100);
	player:getTael(10);
	return true;
end

--��������
function Task_00010504_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10504);
end
