--����Ľ�������
function Task_Accept_00040104()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 8 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40104) or task:HasCompletedTask(40104) or task:HasSubmitedTask(40104) then
		return false;
	end
	if not task:HasSubmitedTask(40103) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040104()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 8 then
		return false;
	end
	if task:HasAcceptedTask(40104) or task:HasCompletedTask(40104) or task:HasSubmitedTask(40104) then
		return false;
	end
	if not task:HasSubmitedTask(40103) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040104()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40104) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040104(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40104) == npcId and Task_Accept_00040104 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40104
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "华丽罩衣";
	elseif task:GetTaskSubmitNpc(40104) == npcId then
		if Task_Submit_00040104() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40104
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "华丽罩衣";
		elseif task:HasAcceptedTask(40104) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40104
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "华丽罩衣";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040104_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你拿来的这些狐皮可是很少见的珍贵皮毛，你带来找我是想做什么呢？";
	action.m_ActionMsg = "是王让我去收集这些皮毛，做一件漂亮的罩衣送给悦意王妃。";
	return action;
end

function Task_00040104_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "原来是这样的，对我来说不是什么难事，而且我正好知道悦意王妃最爱的衣服样式，你稍等一下，我马上就帮你完成。（1分钟后）勇士，已经好了，带去送给悦意王妃吧，她一定会喜欢，会感受到帝释天的好意。";
	action.m_ActionMsg = "哇哦，金光闪闪，真是一件华丽的衣服。";
	return action;
end

function Task_00040104_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士看到你带来的衣服我已经知道答案了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040104_step_table = {
		[1] = Task_00040104_step_01,
		[2] = Task_00040104_step_02,
		[10] = Task_00040104_step_10,
		};

function Task_00040104_step(step)
	if Task_00040104_step_table[step] ~= nil then
		return Task_00040104_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040104_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040104() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15056, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(40104) then
		return false;
	end
	task:AddTaskStep(40104);
	return true;
end



--�ύ����
function Task_00040104_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15056,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(40104) then
		return false;
	end

	package:DelItemAll(15056,1);

	player:AddExp(800);
	player:getCoin(650);
	return true;
end

--��������
function Task_00040104_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15056,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40104);
end
