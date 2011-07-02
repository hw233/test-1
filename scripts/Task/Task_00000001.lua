--����Ľ�������
function Task_Accept_00000001()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(1) or task:HasCompletedTask(1) or task:HasSubmitedTask(1) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000001()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(1) or task:HasCompletedTask(1) or task:HasSubmitedTask(1) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000001()
	if GetPlayer():GetTaskMgr():HasCompletedTask(1) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000001(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(1) == npcId and Task_Accept_00000001 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 1
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "天蒙禅师";
	elseif task:GetTaskSubmitNpc(1) == npcId then
		if Task_Submit_00000001() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 1
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "天蒙禅师";
		elseif task:HasAcceptedTask(1) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 1
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "天蒙禅师";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000001_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "老衲早已在东汉得道，虽说大道无情，可是仍有一丝尘心未泯，这一日老衲静坐枯禅，心中忽有所得，推算出1700年后中华大地将遭遇兵劫，邪道会乘势而起……";
	action.m_ActionMsg = "那我该怎么办？";
	return action;
end

function Task_00000001_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我以佛门无上神通推动元神之力，穿越千年来寻你，须知你是这正邪之战的应劫之人，而蜀中也正是正邪之战的关键之地。你现在速速去湘江边消灭那些追杀李宁父女的水贼，保护好他们父女。";
	action.m_ActionMsg = "好，我这就去。";
	return action;
end

function Task_00000001_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "后面有恶人追逼正紧，这位小哥你为何拦住我们去路？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000001_step_table = {
		[1] = Task_00000001_step_01,
		[2] = Task_00000001_step_02,
		[10] = Task_00000001_step_10,
		};

function Task_00000001_step(step)
	if Task_00000001_step_table[step] ~= nil then
		return Task_00000001_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000001_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000001() then
		return false;
	end
	if not task:AcceptTask(1) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000001_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(1) then
		return false;
	end


	player:AddExp(1000);
	player:getTael(100);
	return true;
end

--��������
function Task_00000001_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(1);
end
