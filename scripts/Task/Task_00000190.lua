--����Ľ�������
function Task_Accept_00000190()
	local player = GetPlayer();
	if player:GetLev() < 95 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(190) or task:HasCompletedTask(190) or task:HasSubmitedTask(190) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000190()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 95 then
		return false;
	end
	if task:HasAcceptedTask(190) or task:HasCompletedTask(190) or task:HasSubmitedTask(190) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000190()
	if GetPlayer():GetTaskMgr():HasCompletedTask(190) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000190(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(190) == npcId and Task_Accept_00000190 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 190
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "神驼乙休";
	elseif task:GetTaskSubmitNpc(190) == npcId then
		if Task_Submit_00000190() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 190
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "神驼乙休";
		elseif task:HasAcceptedTask(190) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 190
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "神驼乙休";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000190_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "老夫居珠峰绝顶已久，这里本是人迹罕至之处，也落得清静，不过自从多年前那毒龙尊者来此开宗立派，就变得不甚太平，特别是他还招募了大批门人，豢养了很多妖兽，老夫也不屑对那些跳梁小丑出手，所以请你出手帮老夫灭掉这些妖兽。";
	action.m_ActionMsg = "我这就去将这些异兽除去。";
	return action;
end

function Task_00000190_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "少侠真是修为深厚，天赋异禀啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000190_step_table = {
		[1] = Task_00000190_step_01,
		[10] = Task_00000190_step_10,
		};

function Task_00000190_step(step)
	if Task_00000190_step_table[step] ~= nil then
		return Task_00000190_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000190_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000190() then
		return false;
	end
	if not task:AcceptTask(190) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000190_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(190) then
		return false;
	end


	player:AddExp(180000);
	return true;
end

--��������
function Task_00000190_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(190);
end
