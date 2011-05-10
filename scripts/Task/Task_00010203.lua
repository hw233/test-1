--����Ľ�������
function Task_Accept_00010203()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10203) or task:HasCompletedTask(10203) or task:HasSubmitedTask(10203) then
		return false;
	end
	if not task:HasSubmitedTask(10202) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010203()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(10203) or task:HasCompletedTask(10203) or task:HasSubmitedTask(10203) then
		return false;
	end
	if not task:HasSubmitedTask(10202) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010203()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10203) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010203(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10203) == npcId and Task_Accept_00010203 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10203
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "甘露的作用";
	elseif task:GetTaskSubmitNpc(10203) == npcId then
		if Task_Submit_00010203() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10203
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "甘露的作用";
		elseif task:HasAcceptedTask(10203) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10203
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "甘露的作用";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010203_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "虽然你新来天族没多久，但是看在你的潜力颇高，我一定要教你认识更多天族的事情。甘露可是我们天族的骄傲，这是只有在我们天庭才能出产的一种物品，能够极大的延长和恢复战斗的能力，对我们天族非常重要，我想你已经听说过曾经的天魔之战了，我们天族在战斗中能够占据主动全靠甘露的支持。";
	action.m_ActionMsg = "那我们为什么还要送甘露给魔族呢？";
	return action;
end

function Task_00010203_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那时帝释天想要娶魔族的悦意公主，可是狂暴的阿修罗却带领魔族打了过来，战争持续了非常久，阿修罗却先抗不住了，派了人来讲和，并愿意将悦意公主送来，我们的帝释天是那么的仁慈，所以决定以持续供应的甘露作为回报。";
	action.m_ActionMsg = "原来是这样，我们的王对贪婪的魔族居然这么仁慈。";
	return action;
end

function Task_00010203_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "了解了天族的故事，勇士有何感想？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010203_step_table = {
		[1] = Task_00010203_step_01,
		[2] = Task_00010203_step_02,
		[10] = Task_00010203_step_10,
		};

function Task_00010203_step(step)
	if Task_00010203_step_table[step] ~= nil then
		return Task_00010203_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010203_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010203() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(10203) then
		return false;
	end
	task:AddTaskStep(10203);
	return true;
end



--�ύ����
function Task_00010203_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(10203) then
		return false;
	end


	player:AddExp(1400);
	player:getCoin(1200);
	return true;
end

--��������
function Task_00010203_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10203);
end
