--����Ľ�������
function Task_Accept_00020504()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 26 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20504) or task:HasCompletedTask(20504) or task:HasSubmitedTask(20504) then
		return false;
	end
	if not task:HasSubmitedTask(20503) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020504()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 26 then
		return false;
	end
	if task:HasAcceptedTask(20504) or task:HasCompletedTask(20504) or task:HasSubmitedTask(20504) then
		return false;
	end
	if not task:HasSubmitedTask(20503) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020504()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20504) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020504(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20504) == npcId and Task_Accept_00020504 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20504
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "力量的试炼";
	elseif task:GetTaskSubmitNpc(20504) == npcId then
		if Task_Submit_00020504() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20504
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "力量的试炼";
		elseif task:HasAcceptedTask(20504) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20504
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "力量的试炼";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020504_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "通过刚刚的战斗，我想勇士你应该已经意识到自己的实力了。";
	action.m_ActionMsg = "我很遗憾。";
	return action;
end

function Task_00020504_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "虽然你努力训练和成长，但是毕竟还不够强壮，不能贸然派你去面对危险，那群夜摩盟里，很可能就有夜叉王和摩可拿在里面，他们的能力可是超出你非常多。这样吧，你去寒山道场找武艺法师，他是一个法术非常高超的法师，他一定会帮助你的。";
	action.m_ActionMsg = "好的，我这就去寻找法师的帮助。";
	return action;
end

function Task_00020504_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "肯定是摧伏派你来找我帮忙的。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020504_step_table = {
		[1] = Task_00020504_step_01,
		[2] = Task_00020504_step_02,
		[10] = Task_00020504_step_10,
		};

function Task_00020504_step(step)
	if Task_00020504_step_table[step] ~= nil then
		return Task_00020504_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020504_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020504() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20504) then
		return false;
	end
	task:AddTaskStep(20504);
	return true;
end



--�ύ����
function Task_00020504_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(20504) then
		return false;
	end


	player:AddExp(3200);
	player:getCoin(5100);
	player:getTael(10);
	return true;
end

--��������
function Task_00020504_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20504);
end
