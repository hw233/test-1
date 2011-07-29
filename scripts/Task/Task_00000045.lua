--����Ľ�������
function Task_Accept_00000045()
	local player = GetPlayer();
	if player:GetLev() < 11 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(45) or task:HasCompletedTask(45) or task:HasSubmitedTask(45) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(44) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(44) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(44) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000045()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 11 then
		return false;
	end
	if task:HasAcceptedTask(45) or task:HasCompletedTask(45) or task:HasSubmitedTask(45) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(44) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(44) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(44) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000045()
	if GetPlayer():GetTaskMgr():HasCompletedTask(45) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000045(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(45) == npcId and Task_Accept_00000045 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 45
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "无间道之殇";
	elseif task:GetTaskSubmitNpc(45) == npcId then
		if Task_Submit_00000045() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 45
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "无间道之殇";
		elseif task:HasAcceptedTask(45) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 45
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "无间道之殇";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000045_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "刚刚我已经将密室的暗门开启，这里是地牢的钥匙，少侠赶快去救人吧，不过那书生笨手笨脚的，希望不会出了什么差错。";
	action.m_ActionMsg = "嗯，我这就去救人，不过你也得小心。";
	return action;
end

function Task_00000045_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 3;
	action.m_NpcMsg = "哎呀，少侠慢走，现在寺内死了几个恶僧，又走失了书生，方丈责怪下来，我也不好交代……不如这样，少侠你插我一剑，这样的话我也好蒙混过关。";
	action.m_ActionMsg = "这……很疼的，你要忍住哦……";
	return action;
end

function Task_00000045_step_03()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "啊……啊啊啊！好痛！你……你怎么插我这里！";
	action.m_ActionMsg = "对不起，我手滑了……要不我在你说的地方再插一下？";
	return action;
end

function Task_00000045_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这位侠士你是来救我的吗？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000045_step_table = {
		[1] = Task_00000045_step_01,
		[2] = Task_00000045_step_02,
		[3] = Task_00000045_step_03,
		[10] = Task_00000045_step_10,
		};

function Task_00000045_step(step)
	if Task_00000045_step_table[step] ~= nil then
		return Task_00000045_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000045_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000045() then
		return false;
	end
	if not task:AcceptTask(45) then
		return false;
	end
	task:AddTaskStep(45);
	return true;
end



--�ύ����
function Task_00000045_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(45) then
		return false;
	end


	player:AddExp(7000);
	return true;
end

--��������
function Task_00000045_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(45);
end
