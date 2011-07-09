--����Ľ�������
function Task_Accept_00000010()
	local player = GetPlayer();
	if player:GetLev() < 10 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10) or task:HasCompletedTask(10) or task:HasSubmitedTask(10) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(9) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(9) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(9) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000010()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 10 then
		return false;
	end
	if task:HasAcceptedTask(10) or task:HasCompletedTask(10) or task:HasSubmitedTask(10) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(9) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(9) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(9) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000010()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000010(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10) == npcId and Task_Accept_00000010 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "昔日仇家";
	elseif task:GetTaskSubmitNpc(10) == npcId then
		if Task_Submit_00000010() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "昔日仇家";
		elseif task:HasAcceptedTask(10) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "昔日仇家";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000010_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "<name>大哥，你行侠仗义的事情我已经听李叔叔说了，不知你现在能不能帮忙寻找我师傅？";
	action.m_ActionMsg = "当然可以，但不知你为何如此焦急，究竟发生了？";
	return action;
end

function Task_00000010_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "少侠不知，村里来了一个形状吓人的和尚，唤做毛太，此人看起来似乎和我师傅有仇，看起来好象传说中的剑仙一流，师傅肯定不是对手，我要赶紧找到师傅，好有所准备。";
	action.m_ActionMsg = "好，我这就同你一起去找他。";
	return action;
end

function Task_00000010_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "毛太是我早年的一个仇人，后来听说他在五台山学了飞剑，厉害无比，我们普通江湖人如何是他的对手？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000010_step_table = {
		[1] = Task_00000010_step_01,
		[2] = Task_00000010_step_02,
		[10] = Task_00000010_step_10,
		};

function Task_00000010_step(step)
	if Task_00000010_step_table[step] ~= nil then
		return Task_00000010_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000010_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000010() then
		return false;
	end
	if not task:AcceptTask(10) then
		return false;
	end
	task:AddTaskStep(10);
	return true;
end



--�ύ����
function Task_00000010_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(10) then
		return false;
	end


	player:AddExp(2222);
	player:getTael(111);
	return true;
end

--��������
function Task_00000010_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(10);
end
