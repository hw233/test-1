--����Ľ�������
function Task_Accept_00000002()
	local player = GetPlayer();
	if player:GetLev() < 2 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(2) or task:HasCompletedTask(2) or task:HasSubmitedTask(2) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(1) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(1) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000002()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 2 then
		return false;
	end
	if task:HasAcceptedTask(2) or task:HasCompletedTask(2) or task:HasSubmitedTask(2) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(1) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(1) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000002()
	if GetPlayer():GetTaskMgr():HasCompletedTask(2) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000002(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(2) == npcId and Task_Accept_00000002 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 2
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "贼首李刚";
	elseif task:GetTaskSubmitNpc(2) == npcId then
		if Task_Submit_00000002() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 2
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "贼首李刚";
		elseif task:HasAcceptedTask(2) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 2
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "贼首李刚";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000002_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我乃是齐鲁三英的李宁，昔年在湖北除掉一个为祸乡里的恶徒陆地飞腾李启明，这帮水贼的首领正是其父浪里飞腾李刚。我寡不敌从，且战且退已是身负重伤，这李刚凶狠异常，<name>你可千万要小心才是。";
	action.m_ActionMsg = "我知道了。";
	return action;
end

function Task_00000002_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这是老夫昔年行走江湖的随身兵刃，虽是凡铁，在老夫手里也惩治过不少恶徒，我看<name>你空手而来，想必是武艺高强，不过还是请你收下此剑。";
	action.m_ActionMsg = "多谢老丈，我一定不会放过这个恶人。";
	return action;
end

function Task_00000002_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "少侠身手真是高明，老朽我是白练半辈子武艺了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000002_step_table = {
		[1] = Task_00000002_step_01,
		[2] = Task_00000002_step_02,
		[10] = Task_00000002_step_10,
		};

function Task_00000002_step(step)
	if Task_00000002_step_table[step] ~= nil then
		return Task_00000002_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000002_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000002() then
		return false;
	end
	if not task:AcceptTask(2) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000002_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(2) then
		return false;
	end


	player:AddExp(1000);
	return true;
end

--��������
function Task_00000002_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(2);
end
