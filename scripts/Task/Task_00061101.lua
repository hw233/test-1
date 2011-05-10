--����Ľ�������
function Task_Accept_00061101()
	local player = GetPlayer();
	if player:GetLev() < 48 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61101) or task:HasCompletedTask(61101) or task:HasSubmitedTask(61101) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061101()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 48 then
		return false;
	end
	if task:HasAcceptedTask(61101) or task:HasCompletedTask(61101) or task:HasSubmitedTask(61101) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00061101()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61101) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061101(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61101) == npcId and Task_Accept_00061101 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61101
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "菜刀路之遥";
	elseif task:GetTaskSubmitNpc(61101) == npcId then
		if Task_Submit_00061101() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61101
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "菜刀路之遥";
		elseif task:HasAcceptedTask(61101) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61101
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "菜刀路之遥";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061101_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "哈哈我菜刀路之遥又回来了。";
	action.m_ActionMsg = "有什么事情你说吧，看我能不能帮到你。";
	return action;
end

function Task_00061101_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我菜刀路之遥的菜刀，在我睡觉的时候被码头废墟的渔人小偷偷走了，没了菜刀我还咋被称为菜刀路之遥啊，而且我想抢也抢不回来了，能不能麻烦你从那些渔人小偷那里抢回我的菜刀啊。";
	action.m_ActionMsg = "原来是遭了小偷啊，我想我应该能搞定那些渔人小偷。";
	return action;
end

function Task_00061101_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "哈哈我菜刀路之遥又回来了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061101_step_table = {
		[1] = Task_00061101_step_01,
		[2] = Task_00061101_step_02,
		[10] = Task_00061101_step_10,
		};

function Task_00061101_step(step)
	if Task_00061101_step_table[step] ~= nil then
		return Task_00061101_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061101_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061101() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61101) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15010,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15010, itemNum, 1);
		end
		task:AddTaskStep2(61101, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00061101_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15010,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(61101) then
		return false;
	end

	package:DelItemAll(15010,1);

	player:AddExp(48000);
	player:getCoin(34000);
	return true;
end

--��������
function Task_00061101_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15010,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61101);
end
