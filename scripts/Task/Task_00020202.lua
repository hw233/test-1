--����Ľ�������
function Task_Accept_00020202()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20202) or task:HasCompletedTask(20202) or task:HasSubmitedTask(20202) then
		return false;
	end
	if not task:HasSubmitedTask(20201) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020202()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(20202) or task:HasCompletedTask(20202) or task:HasSubmitedTask(20202) then
		return false;
	end
	if not task:HasSubmitedTask(20201) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020202()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20202) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020202(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20202) == npcId and Task_Accept_00020202 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20202
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "传递密信";
	elseif task:GetTaskSubmitNpc(20202) == npcId then
		if Task_Submit_00020202() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20202
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "传递密信";
		elseif task:HasAcceptedTask(20202) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20202
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "传递密信";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020202_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "如果你是战神因达罗派来的勇士，那么你是值得信任的人。你可知道我们的战神因达罗，是悦 意公主的爱人，他们都是我们魔族了不起的人物，很多年前自以为是的帝释天想要娶我们火神毗摩质多罗的女儿悦意公主为妻，王阿修罗和整个魔族都受不了帝释天的行为，而发起了天魔之战。";
	action.m_ActionMsg = "那结果怎样了呢？我很受感动，我会帮你送去的。";
	return action;
end

function Task_00020202_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "战神是那么的勇敢不屈，以至于在战斗中负了伤，而悦意公主也是为了大局着想，不愿意看到战争毁灭性的后果，于是以自己为条件讲和，然后去了天庭，她与因达罗也因此分开不能再相见，我是主动申请冒着危险潜入天庭与公主取得联系的，请你把公主的密信带回给因达罗吧。";
	action.m_ActionMsg = "我很受感动，我会帮你送去的。";
	return action;
end

function Task_00020202_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你带回了公主的消息吗？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020202_step_table = {
		[1] = Task_00020202_step_01,
		[2] = Task_00020202_step_02,
		[10] = Task_00020202_step_10,
		};

function Task_00020202_step(step)
	if Task_00020202_step_table[step] ~= nil then
		return Task_00020202_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020202_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020202() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15027, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(20202) then
		return false;
	end
	task:AddTaskStep(20202);
	return true;
end



--�ύ����
function Task_00020202_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15027,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(20202) then
		return false;
	end

	package:DelItemAll(15027,1);

	player:AddExp(1400);
	player:getCoin(1000);
	return true;
end

--��������
function Task_00020202_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15027,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20202);
end
